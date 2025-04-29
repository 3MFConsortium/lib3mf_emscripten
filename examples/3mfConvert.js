import initLib3MF from '../build_bindings/lib3mfEmscriptenTest.js';
import fs from 'fs';

const inputFileName = process.argv[2];

if (!inputFileName) {
    console.log("Usage:");
    console.log("Convert 3MF to STL: node 3mfConvert.js model.3mf");
    console.log("Convert STL to 3MF: node 3mfConvert.js model.stl");
    process.exit(1);
}

function findExtension(filename) {
    const idx = filename.lastIndexOf('.');
    if (idx !== -1) {
        return filename.substring(idx).toLowerCase();
    }
    return "";
}

initLib3MF().then(lib3mf => {
    console.log("✅ lib3mf WASM module initialized.");

    const wrapper = new lib3mf.CWrapper();
    const libVersion = wrapper.GetLibraryVersion();
    console.log(`✅ lib3mf Version: ${libVersion.Major}.${libVersion.Minor}.${libVersion.Micro}`);

    const extension = findExtension(inputFileName);
    let readerName = "";
    let writerName = "";
    let newExtension = "";

    if (extension === ".stl") {
        readerName = "stl";
        writerName = "3mf";
        newExtension = ".3mf";
    } else if (extension === ".3mf") {
        readerName = "3mf";
        writerName = "stl";
        newExtension = ".stl";
    } else {
        console.log(`Unknown input file extension: ${extension}`);
        process.exit(1);
    }

    const outputFileName = inputFileName.substring(0, inputFileName.length - extension.length) + newExtension;

    const model = wrapper.CreateModel();
    const reader = model.QueryReader(readerName);

    console.log(`✅ Reading ${inputFileName}...`);
    const inputBuffer = fs.readFileSync(inputFileName);
    lib3mf.FS.writeFile(`/input${extension}`, inputBuffer);
    reader.ReadFromFile(`/input${extension}`);

    const writer = model.QueryWriter(writerName);

    console.log(`✅ Writing ${outputFileName}...`);
    const outputPath = `/output${newExtension}`;
    writer.WriteToFile(outputPath);

    const outputBuffer = lib3mf.FS.readFile(outputPath);
    fs.writeFileSync(outputFileName, outputBuffer);

    console.log("✅ Done");
});