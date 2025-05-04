import initLib3MF from '../build_bindings/lib3mfEmscriptenTest.js';
import fs from 'fs';

// Function to create gyroid implicit function
function createGyroidFunction(model, lib3mf) {
    const gyroidFunction = model.AddImplicitFunction();
    gyroidFunction.SetDisplayName("gyroid");

    // Add input position
    const inputPos = gyroidFunction.AddInput("pos", "position", lib3mf.eImplicitPortType.Vector);

    // Add decompose vector node
    const decomposePos = gyroidFunction.AddDecomposeVectorNode("decomposePos", "decompose pos", "group_a");
    gyroidFunction.AddLinkByNames("pos", "decomposePos.a");

    // Add compose vector node for YZX
    const composeYZX = gyroidFunction.AddComposeVectorNode("composeYZX", "compose yzx", "group_a");
    gyroidFunction.AddLinkByNames("decomposePos.z", "composeYZX.y");
    gyroidFunction.AddLinkByNames("decomposePos.y", "composeYZX.x");
    gyroidFunction.AddLinkByNames("decomposePos.x", "composeYZX.z");

    // Add sin node
    const sinNode = gyroidFunction.AddSinNode(
        "sin", 
        lib3mf.eImplicitNodeConfiguration.VectorToVector, 
        "sinus", 
        "group_a"
    );
    gyroidFunction.AddLinkByNames("pos", "sin.a");

    // Add cos node
    const cosNode = gyroidFunction.AddCosNode(
        "cos", 
        lib3mf.eImplicitNodeConfiguration.VectorToVector, 
        "cosinus", 
        "group_a"
    );
    gyroidFunction.AddLinkByNames("composeYZX.result", "cos.a");

    // Add dot product node
    const dotNode = gyroidFunction.AddDotNode("dot", "dot product", "group_a");
    gyroidFunction.AddLinkByNames("sin.result", "dot.a");
    gyroidFunction.AddLinkByNames("cos.result", "dot.b");

    // Add absolute value node
    const absNode = gyroidFunction.AddAbsNode(
        "abs", 
        lib3mf.eImplicitNodeConfiguration.ScalarToScalar, 
        "abs", 
        "group_a"
    );
    gyroidFunction.AddLinkByNames("dot.result", "abs.a");

    // Add subtraction node
    const subtractionNode = gyroidFunction.AddSubtractionNode(
        "sub", 
        lib3mf.eImplicitNodeConfiguration.ScalarToScalar, 
        "substract", 
        "group_a"
    );

    // Add constant node for half thickness
    const halfThicknessNode = gyroidFunction.AddConstantNode(
        "half_thickness", 
        "half of the thickness", 
        "group_a"
    );
    halfThicknessNode.SetConstant(0.4);

    // Link the nodes
    gyroidFunction.AddLinkByNames("abs.result", "sub.a");
    gyroidFunction.AddLinkByNames("half_thickness.value", "sub.b");

    // Add output
    const output = gyroidFunction.AddOutput(
        "shape", 
        "signed distance to the surface", 
        lib3mf.eImplicitPortType.Scalar
    );
    gyroidFunction.AddLinkByNames("sub.result", "shape");

    return gyroidFunction;
}

// Function to create a box mesh
function createBoxAsMesh(model, lib3mf, sizeX, sizeY, sizeZ) {
    const meshObject = model.AddMeshObject();
    meshObject.SetName("Box");

    // Helper to create a position vector
    const makeVec3 = (x, y, z) => {
        const pos = new lib3mf.sPosition();
        pos.set_Coordinates0(x);
        pos.set_Coordinates1(y);
        pos.set_Coordinates2(z);
        return pos;
    };

    // Helper to create a triangle
    const makeTriangle = (v1, v2, v3) => {
        const tri = new lib3mf.sTriangle();
        tri.set_Indices0(v1);
        tri.set_Indices1(v2);
        tri.set_Indices2(v3);
        return tri;
    };

    // Create vertices
    const vertices = [
        makeVec3(0.0, 0.0, 0.0),
        makeVec3(sizeX, 0.0, 0.0),
        makeVec3(sizeX, sizeY, 0.0),
        makeVec3(0.0, sizeY, 0.0),
        makeVec3(0.0, 0.0, sizeZ),
        makeVec3(sizeX, 0.0, sizeZ),
        makeVec3(sizeX, sizeY, sizeZ),
        makeVec3(0.0, sizeY, sizeZ)
    ];

    // Add vertices to mesh
    vertices.forEach(v => meshObject.AddVertex(v));

    // Define triangles (similar to the C++ example)
    const triangleIndices = [
        [0, 1, 2], [0, 2, 3], // Bottom face
        [4, 5, 6], [4, 6, 7], // Top face
        [0, 1, 5], [0, 5, 4], // Front face
        [1, 2, 6], [1, 6, 5], // Right face
        [2, 3, 7], [2, 7, 6], // Back face
        [3, 0, 4], [3, 4, 7]  // Left face
    ];

    // Add triangles to mesh
    triangleIndices.forEach(([i0, i1, i2]) => {
        const tri = makeTriangle(i0, i1, i2);
        meshObject.AddTriangle(tri);
    });

    return meshObject;
}

// Main function
initLib3MF().then(lib3mf => {
    console.log("✅ lib3mf WASM module initialized.");
    
    const wrapper = new lib3mf.CWrapper();
    const model = wrapper.CreateModel();

    // Create the box mesh
    const box = createBoxAsMesh(model, lib3mf, 50.0, 23.0, 45.0);
    
    // Create the gyroid function
    const gyroidFunction = createGyroidFunction(model, lib3mf);
    
    // Create the level set with the gyroid function
    const theLevelSet = model.AddLevelSet();
    theLevelSet.SetFunction(gyroidFunction);
    theLevelSet.SetChannelName("shape");
    theLevelSet.SetMesh(box);
    theLevelSet.SetMeshBBoxOnly(true); // Treating the mesh as a bounding box makes evaluation faster
    
    // Add a build item for the level set
    model.AddBuildItem(theLevelSet, wrapper.GetIdentityTransform());
    
    // Write the model to a file
    const writer = model.QueryWriter("3mf");
    const outputPath = "/GyroidBox.3mf";
    writer.WriteToFile(outputPath);
    
    // Save the file to disk
    const buffer = lib3mf.FS.readFile(outputPath);
    fs.writeFileSync("./GyroidBox.3mf", buffer);
    console.log("✅ 3MF file with gyroid box written to GyroidBox.3mf");
});
