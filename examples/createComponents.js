import initLib3MF from '../build_bindings/lib3mfEmscriptenTest.js';
import fs from 'fs';

initLib3MF().then(lib3mf => {
    console.log("✅ lib3mf WASM module initialized.");

    const wrapper = new lib3mf.CWrapper();

    const libVersion = wrapper.GetLibraryVersion();
    console.log(`✅ lib3mf Version: ${libVersion.Major}.${libVersion.Minor}.${libVersion.Micro}`);

    const model = wrapper.CreateModel();
    console.log("✅ Created new model");

    const meshObject = model.AddMeshObject();
    meshObject.SetName("Box");
    console.log("✅ Created mesh object: 'Box'");

    // Helper: Create a translation matrix
    function createTranslationMatrix(x, y, z) {
        const transform = new lib3mf.sTransform();
        transform.set_Fields_0_0(1.0); transform.set_Fields_0_1(0.0); transform.set_Fields_0_2(0.0);
        transform.set_Fields_1_0(0.0); transform.set_Fields_1_1(1.0); transform.set_Fields_1_2(0.0);
        transform.set_Fields_2_0(0.0); transform.set_Fields_2_1(0.0); transform.set_Fields_2_2(1.0);
        transform.set_Fields_3_0(x);   transform.set_Fields_3_1(y);   transform.set_Fields_3_2(z);
        return transform;
    }

    function createVertex(mesh, x, y, z) {
        const pos = lib3mf.sPosition.fromStruct({ Coordinates0: x, Coordinates1: y, Coordinates2: z });
        return mesh.AddVertex(pos);
    }

    function addTriangle(mesh, i0, i1, i2) {
        const tri = lib3mf.sTriangle.fromStruct({ Indices0: i0, Indices1: i1, Indices2: i2 });
        mesh.AddTriangle(tri);
    }

    const sizeX = 10.0, sizeY = 20.0, sizeZ = 30.0;

    const v = [
        createVertex(meshObject, 0, 0, 0),
        createVertex(meshObject, sizeX, 0, 0),
        createVertex(meshObject, sizeX, sizeY, 0),
        createVertex(meshObject, 0, sizeY, 0),
        createVertex(meshObject, 0, 0, sizeZ),
        createVertex(meshObject, sizeX, 0, sizeZ),
        createVertex(meshObject, sizeX, sizeY, sizeZ),
        createVertex(meshObject, 0, sizeY, sizeZ)
    ];
    console.log(`✅ Created ${v.length} vertices`);

    const tris = [
        [2,1,0], [0,3,2], [4,5,6], [6,7,4],
        [0,1,5], [5,4,0], [2,3,7], [7,6,2],
        [1,2,6], [6,5,1], [3,0,4], [4,7,3]
    ];
    tris.forEach(([i0, i1, i2]) => addTriangle(meshObject, v[i0], v[i1], v[i2]));
    console.log(`✅ Created ${tris.length} triangles`);

    const componentsObject = model.AddComponentsObject();

    const identity = createTranslationMatrix(0.0, 0.0, 0.0);
    const t1 = createTranslationMatrix(40.0, 60.0, 80.0);
    const t2 = createTranslationMatrix(120.0, 30.0, 70.0);

    const c0 = componentsObject.AddComponent(meshObject, identity);
    const c1 = componentsObject.AddComponent(meshObject, t1);
    const c2 = componentsObject.AddComponent(meshObject, t2);
    console.log("✅ Added mesh object 3 times with different transforms");

    // Clean up leaked components
    // (optional: you can delete c0, c1, c2 if needed)

    const buildItem = model.AddBuildItem(componentsObject, identity);
    console.log("✅ Added components object to build");
    buildItem.delete();

    const writer3mf = model.QueryWriter("3mf");
    const writerStl = model.QueryWriter("stl");

    const path3mf = "/components.3mf";
    const pathStl = "/components.stl";

    writer3mf.WriteToFile(path3mf);
    writerStl.WriteToFile(pathStl);

    fs.writeFileSync("components.3mf", lib3mf.FS.readFile(path3mf));
    fs.writeFileSync("components.stl", lib3mf.FS.readFile(pathStl));
    console.log("✅ Wrote components.3mf and components.stl");
});
