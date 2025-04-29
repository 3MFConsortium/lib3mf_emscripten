import initLib3MF from '../build_bindings/lib3mfEmscriptenTest.js';
import fs from 'fs';

initLib3MF().then(lib3mf => {
    console.log("✅ lib3mf WASM module initialized.");

    const wrapper = new lib3mf.CWrapper();
    const version = wrapper.GetLibraryVersion();
    console.log(`✅ lib3mf Version: ${version.Major}.${version.Minor}.${version.Micro}`);

    const model = wrapper.CreateModel();
    const meshObject = model.AddMeshObject();
    meshObject.SetName("Colored Box");
    console.log("✅ Created mesh object: 'Colored Box'");

    function createVertex(mesh, x, y, z) {
        const pos = lib3mf.sPosition.fromStruct({ Coordinates0: x, Coordinates1: y, Coordinates2: z });
        return mesh.AddVertex(pos);
    }

    function addTriangle(mesh, i0, i1, i2) {
        const tri = lib3mf.sTriangle.fromStruct({ Indices0: i0, Indices1: i1, Indices2: i2 });
        mesh.AddTriangle(tri);
        return tri;
    }

    function createTriangleColor(colorGroup, colorId0, colorId1, colorId2) {
        const triangleProps = new lib3mf.sTriangleProperties();
        triangleProps.set_ResourceID(colorGroup.GetResourceID());
        triangleProps.set_PropertyIDs0(colorId0);
        triangleProps.set_PropertyIDs1(colorId1);
        triangleProps.set_PropertyIDs2(colorId2);
        return triangleProps;
    }

    const fSizeX = 100.0, fSizeY = 200.0, fSizeZ = 300.0;

    const v = [
        createVertex(meshObject, 0.0, 0.0, 0.0),
        createVertex(meshObject, fSizeX, 0.0, 0.0),
        createVertex(meshObject, fSizeX, fSizeY, 0.0),
        createVertex(meshObject, 0.0, fSizeY, 0.0),
        createVertex(meshObject, 0.0, 0.0, fSizeZ),
        createVertex(meshObject, fSizeX, 0.0, fSizeZ),
        createVertex(meshObject, fSizeX, fSizeY, fSizeZ),
        createVertex(meshObject, 0.0, fSizeY, fSizeZ)
    ];
    console.log("✅ Created 8 vertices");

    const triangleIndices = [
        [2, 1, 0], [0, 3, 2], [4, 5, 6], [6, 7, 4],
        [0, 1, 5], [5, 4, 0], [2, 3, 7], [7, 6, 2],
        [1, 2, 6], [6, 5, 1], [3, 0, 4], [4, 7, 3]
    ];
    triangleIndices.forEach(([i0, i1, i2]) => addTriangle(meshObject, v[i0], v[i1], v[i2]));
    console.log("✅ Created 12 triangles");

    const colorGroup = model.AddColorGroup();

    const idRed    = colorGroup.AddColor(wrapper.RGBAToColor(255, 0, 0, 255));
    const idGreen  = colorGroup.AddColor(wrapper.RGBAToColor(0, 255, 0, 255));
    const idBlue   = colorGroup.AddColor(wrapper.RGBAToColor(0, 0, 255, 255));
    const idOrange = colorGroup.AddColor(wrapper.RGBAToColor(255, 128, 0, 255));
    const idYellow = colorGroup.AddColor(wrapper.RGBAToColor(255, 255, 0, 255));

    const colorRed    = createTriangleColor(colorGroup, idRed, idRed, idRed);
    const colorGreen  = createTriangleColor(colorGroup, idGreen, idGreen, idGreen);
    const colorBlue   = createTriangleColor(colorGroup, idBlue, idBlue, idBlue);
    const colorMix1   = createTriangleColor(colorGroup, idOrange, idRed, idYellow);
    const colorMix2   = createTriangleColor(colorGroup, idYellow, idGreen, idOrange);

    meshObject.SetTriangleProperties(0, colorRed);
    meshObject.SetTriangleProperties(1, colorRed);
    meshObject.SetTriangleProperties(2, colorGreen);
    meshObject.SetTriangleProperties(3, colorGreen);
    meshObject.SetTriangleProperties(4, colorBlue);
    meshObject.SetTriangleProperties(5, colorBlue);

    meshObject.SetTriangleProperties(6, colorMix1);
    meshObject.SetTriangleProperties(7, colorMix2);
    meshObject.SetTriangleProperties(8, colorMix1);
    meshObject.SetTriangleProperties(9, colorMix2);
    meshObject.SetTriangleProperties(10, colorMix1);
    meshObject.SetTriangleProperties(11, colorMix2);
    console.log("✅ Applied triangle colors");

    meshObject.SetObjectLevelProperty(colorRed.get_ResourceID(), colorRed.get_PropertyIDs0());

    const identity = wrapper.GetIdentityTransform();
    model.AddBuildItem(meshObject, identity);
    console.log("✅ Added mesh to build");

    const writer = model.QueryWriter("3mf");
    const outputPath = "/colorcube.3mf";
    writer.WriteToFile(outputPath);
    fs.writeFileSync("colorcube.3mf", lib3mf.FS.readFile(outputPath));
    console.log("✅ Wrote colorcube.3mf");
});