import initLib3MF from '../build_bindings/lib3mfEmscriptenTest.js';
import fs from 'fs';

initLib3MF().then(lib3mf => {
    console.log("✅ lib3mf WASM module initialized.");

    const wrapper = new lib3mf.CWrapper();
    const version = wrapper.GetLibraryVersion();
    console.log(`✅ lib3mf Version: ${version.Major}.${version.Minor}.${version.Micro}`);

    const model = wrapper.CreateModel();
    const meshObject = model.AddMeshObject();
    meshObject.SetName("Beamlattice");
    console.log("✅ Created mesh object: 'Beamlattice'");

    function createVertex(mesh, x, y, z) {
        const pos = lib3mf.sPosition.fromStruct({ Coordinates0: x, Coordinates1: y, Coordinates2: z });
        return mesh.AddVertex(pos);
    }

    const fSizeX = 100.0, fSizeY = 200.0, fSizeZ = 300.0;

    const vertices = [
        createVertex(meshObject, 0, 0, 0),
        createVertex(meshObject, fSizeX, 0, 0),
        createVertex(meshObject, fSizeX, fSizeY, 0),
        createVertex(meshObject, 0, fSizeY, 0),
        createVertex(meshObject, 0, 0, fSizeZ),
        createVertex(meshObject, fSizeX, 0, fSizeZ),
        createVertex(meshObject, fSizeX, fSizeY, fSizeZ),
        createVertex(meshObject, 0, fSizeY, fSizeZ)
    ];
    console.log("✅ Created 8 vertices");

    function toCapModeEnum(str) {
        const s = str.toLowerCase();
        if (s === "butt") return lib3mf.eBeamLatticeCapMode.Butt;
        if (s === "sphere") return lib3mf.eBeamLatticeCapMode.Sphere;
        if (s === "hemisphere") return lib3mf.eBeamLatticeCapMode.HemiSphere;
        throw new Error("Unknown cap mode: " + str);
    }

    function createBeam(i0, i1, r0, r1, c0, c1) {
        const beam = new lib3mf.sBeam();
        beam.set_Indices0(i0);
        beam.set_Indices1(i1);
        beam.set_Radii0(r0);
        beam.set_Radii1(r1);
        beam.set_CapModes0(toCapModeEnum(c0));
        beam.set_CapModes1(toCapModeEnum(c1));
        return beam;
    }

    const r0 = 1.0, r1 = 1.5, r2 = 2.0, r3 = 2.5;

    const beams = [
        createBeam(2, 1, r0, r0, "Butt", "Butt"),
        createBeam(0, 3, r0, r1, "Sphere", "Butt"),
        createBeam(4, 5, r0, r2, "Sphere", "Butt"),
        createBeam(6, 7, r0, r3, "HemiSphere", "Butt"),
        createBeam(0, 1, r1, r0, "HemiSphere", "Butt"),
        createBeam(5, 4, r1, r1, "Sphere", "HemiSphere"),
        createBeam(2, 3, r1, r2, "Sphere", "Sphere"),
        createBeam(7, 6, r1, r3, "Butt", "Butt"),
        createBeam(1, 2, r2, r2, "Butt", "Butt"),
        createBeam(6, 5, r2, r3, "HemiSphere", "Butt"),
        createBeam(3, 0, r3, r0, "Butt", "Sphere"),
        createBeam(4, 7, r3, r1, "HemiSphere", "HemiSphere")
    ];
    console.log(`✅ Created ${beams.length} beams`);

    const beamLattice = meshObject.BeamLattice();
    beams.forEach(beam => {
        beamLattice.AddBeam(beam);
    });
    beamLattice.SetMinLength(0.005);
    console.log("✅ Applied beam lattice");

    const identity = wrapper.GetIdentityTransform();
    model.AddBuildItem(meshObject, identity);

    const writer = model.QueryWriter("3mf");
    const outputPath = "/beamlattice.3mf";
    writer.WriteToFile(outputPath);
    fs.writeFileSync("beamlattice.3mf", lib3mf.FS.readFile(outputPath));
    console.log("✅ Wrote beamlattice.3mf");
});