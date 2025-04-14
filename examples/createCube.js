import initLib3MF from '../build_bindings/lib3mfEmscriptenTest.js';
import fs from 'fs';

initLib3MF().then(lib3mf => {
    console.log("✅ lib3mf WASM module initialized.");

    const wrapper = new lib3mf.CWrapper();
    const model = wrapper.CreateModel();
    const meshObject = model.AddMeshObject();

    const fSizeX = 100.0, fSizeY = 200.0, fSizeZ = 300.0;

    // Helper to create a sPositionWrapper
    const makeVec3 = (x, y, z) => {
        const pos = new lib3mf.sPosition();
        pos.set_Coordinates0(x);
        pos.set_Coordinates1(y);
        pos.set_Coordinates2(z);
        return pos;
    };

    // Helper to create a sTriangleWrapper
    const makeTriangle = (v1, v2, v3) => {
        const tri = new lib3mf.sTriangle();
        tri.set_Indices0(v1);
        tri.set_Indices1(v2);
        tri.set_Indices2(v3);
        return tri;
    };

    const vertices = [
        makeVec3(0, 0, 0),
        makeVec3(fSizeX, 0, 0),
        makeVec3(fSizeX, fSizeY, 0),
        makeVec3(0, fSizeY, 0),
        makeVec3(0, 0, fSizeZ),
        makeVec3(fSizeX, 0, fSizeZ),
        makeVec3(fSizeX, fSizeY, fSizeZ),
        makeVec3(0, fSizeY, fSizeZ),
    ];

    vertices.forEach(v => meshObject.AddVertex(v));

    const triangleIndices = [
        [2, 1, 0], [0, 3, 2], [4, 5, 6], [6, 7, 4],
        [0, 1, 5], [5, 4, 0], [2, 3, 7], [7, 6, 2],
        [1, 2, 6], [6, 5, 1], [3, 0, 4], [4, 7, 3],
    ];

    triangleIndices.forEach(([i0, i1, i2]) => {
        const tri = makeTriangle(i0, i1, i2);
        meshObject.AddTriangle(tri);
    });

    const transform = wrapper.GetIdentityTransform();
    model.AddBuildItem(meshObject, transform);

    const writer = model.QueryWriter("3mf");
    const outputPath = "/generated_cube.3mf";
    writer.WriteToFile(outputPath);

    const buffer = lib3mf.FS.readFile(outputPath);
    fs.writeFileSync("./generated_cube.3mf", buffer);
    console.log("✅ 3MF file with cube written to generated_cube.3mf");
});