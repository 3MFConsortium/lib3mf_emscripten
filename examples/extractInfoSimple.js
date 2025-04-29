import initLib3MF from '../build_bindings/lib3mfEmscriptenTest.js';
import fs from 'fs';

initLib3MF().then(lib3mf => {
  console.log("✅ lib3mf WASM module initialized.");

  // Create wrapper instance first
  const wrapper = new lib3mf.CWrapper();

  // Then call CreateModel on wrapper
  const model = wrapper.CreateModel();
  const reader = model.QueryReader("3mf");

  // Read file buffer and write to Emscripten's virtual filesystem
  const fileBuffer = fs.readFileSync("./cube.3mf");
  lib3mf.FS.writeFile("/cube.3mf", fileBuffer);

  reader.ReadFromFile("/cube.3mf");

  let meshCount = 0;
  const objectIterator = model.GetObjects();
  while (objectIterator.MoveNext()) {
    const obj = objectIterator.GetCurrentObject();
    const resourceID = obj.GetResourceID();
    const isMesh = obj.IsMeshObject();
    const isComponent = obj.IsComponentsObject();

    if (isMesh) {
        console.log(`Mesh Object: ${resourceID}`);
        meshCount++;
    } else if (isComponent) {
        console.log(`Components Object: ${resourceID}`);
    } else {
        console.log(`Unknown Object: ${resourceID}`);
    }
}

  console.log(`✅ Done. Found ${meshCount} mesh object(s).`);
});
