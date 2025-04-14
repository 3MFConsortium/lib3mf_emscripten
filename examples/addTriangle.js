import initLib3MF from '../build_bindings/lib3mfEmscriptenTest.js';
import fs from 'fs';

initLib3MF().then(lib3mf => {
    console.log("✅ lib3mf WASM module initialized");

    const wrapper = new lib3mf.CWrapper();
    const model = wrapper.CreateModel();
    const meshObject = model.AddMeshObject();

    // Helper: Create vertex with struct wrapper
    function createVertexAndReturnIndex(mesh, x, y, z) {
        const pos = lib3mf.sPosition.fromStruct({
            Coordinates0: x,
            Coordinates1: y,
            Coordinates2: z
        });
        const index = mesh.AddVertex(pos);
        if (index === undefined || index === null) {
            throw new Error(`Failed to add vertex at (${x}, ${y}, ${z})`);
        }
        return index;
    }

    // Helper: Add triangle with struct wrapper
    function addTriangle(mesh, i0, i1, i2) {
        const tri = lib3mf.sTriangle.fromStruct({
            Indices0: i0,
            Indices1: i1,
            Indices2: i2
        });
        mesh.AddTriangle(tri);
    }

    // Create vertices
    const p1 = createVertexAndReturnIndex(meshObject, 0, 0, 0);
    const p2 = createVertexAndReturnIndex(meshObject, 0.5, 0, 0);
    const p3 = createVertexAndReturnIndex(meshObject, 0.5, 0.5, 1);

    // Create triangle
    addTriangle(meshObject, p1, p2, p3);

    // Write file
    const writer = model.QueryWriter("3mf");
    const path = "/triangle.3mf";
    writer.WriteToFile(path);

    const buffer = lib3mf.FS.readFile(path);
    fs.writeFileSync("./triangle.3mf", buffer);
    console.log("✅ Wrote triangle.3mf successfully");
}).catch(err => {
    console.error("❌ Error:", err.message);
    process.exit(1);
});