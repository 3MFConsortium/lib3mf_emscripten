#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <array>
#include <unordered_map>
#include <sstream>
#include <functional>
#include <stdexcept>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <Bindings/Cpp/lib3mf_implicit.hpp>

using namespace Lib3MF;
using namespace emscripten;  // Add this to use emscripten namespace directly

// Utility functions to create vertices and triangles
sLib3MFPosition fnCreateVertex(float x, float y, float z) {
    sLib3MFPosition result;
    result.m_Coordinates[0] = x;
    result.m_Coordinates[1] = y;
    result.m_Coordinates[2] = z;
    return result;
}

sLib3MFTriangle fnCreateTriangle(int v0, int v1, int v2) {
    sLib3MFTriangle result;
    result.m_Indices[0] = v0;
    result.m_Indices[1] = v1;
    result.m_Indices[2] = v2;
    return result;
}

sLib3MFTriangleProperties fnCreateTriangleColor(PColorGroup colorGroup, Lib3MF_uint32 colorID1, Lib3MF_uint32 colorID2, Lib3MF_uint32 colorID3)
{
    sLib3MFTriangleProperties sTriangleProperty;
    sTriangleProperty.m_ResourceID = colorGroup->GetResourceID();
    sTriangleProperty.m_PropertyIDs[0] = colorID1;
    sTriangleProperty.m_PropertyIDs[1] = colorID2;
    sTriangleProperty.m_PropertyIDs[2] = colorID3;
    return sTriangleProperty;
}

sLib3MFTransform createTranslationMatrix(float x, float y, float z) {
    sLib3MFTransform mMatrix;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            mMatrix.m_Fields[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }

    mMatrix.m_Fields[3][0] = x;
    mMatrix.m_Fields[3][1] = y;
    mMatrix.m_Fields[3][2] = z;

    return mMatrix;
}

void printVersion(PWrapper wrapper) {
    Lib3MF_uint32 nMajor, nMinor, nMicro;
    wrapper->GetLibraryVersion(nMajor, nMinor, nMicro);
    std::cout << "lib3mf version = " << nMajor << "." << nMinor << "." << nMicro;
    std::string sReleaseInfo, sBuildInfo;
    if (wrapper->GetPrereleaseInformation(sReleaseInfo)) {
        std::cout << "-" << sReleaseInfo;
    }
    if (wrapper->GetBuildInformation(sBuildInfo)) {
        std::cout << "+" << sBuildInfo;
    }
    std::cout << std::endl;
}

std::tuple<std::string, std::vector<sLib3MFPosition>, std::vector<sLib3MFTriangle>>
readSTL(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open STL file.");
    }

    std::string meshName;

    // Lambda for the hash function
    auto arrayHash = [](const std::array<double, 3>& arr) {
        std::hash<double> hasher;
        return hasher(arr[0]) ^ hasher(arr[1]) ^ hasher(arr[2]);
    };

    // Lambda to sanitize the binary STL header
    auto sanitizeHeader = [](const char* header, size_t length) {
        std::string sanitized;
        for (size_t i = 0; i < length; ++i) {
            if (std::isprint(static_cast<unsigned char>(header[i]))) {
                sanitized += header[i];
            }
        }
        return sanitized;
    };

    // Lambda to read binary STL files
    auto readBinarySTL = [&file, &arrayHash, &sanitizeHeader, &meshName]() {
        char header[80];
        file.read(header, 80);

        // Sanitize the header to extract the mesh name
        meshName = sanitizeHeader(header, 80);

        uint32_t numTriangles;
        file.read(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));

        std::vector<std::array<double, 3>> vertices;
        std::vector<std::array<unsigned int, 3>> triangles;
        std::unordered_map<std::array<double, 3>, unsigned int, decltype(arrayHash)> vertexMap(0, arrayHash);

        for (uint32_t i = 0; i < numTriangles; ++i) {
            float normal[3];
            file.read(reinterpret_cast<char*>(normal), 3 * sizeof(float));

            std::array<unsigned int, 3> triangle;
            for (int j = 0; j < 3; ++j) {
                float vertex[3];
                file.read(reinterpret_cast<char*>(vertex), 3 * sizeof(float));
                std::array<double, 3> vertexArray = {vertex[0], vertex[1], vertex[2]};

                auto it = vertexMap.find(vertexArray);
                if (it == vertexMap.end()) {
                    unsigned int index = vertices.size();
                    vertices.push_back(vertexArray);
                    vertexMap[vertexArray] = index;
                    triangle[j] = index;
                } else {
                    triangle[j] = it->second;
                }
            }
            triangles.push_back(triangle);

            uint16_t attributeByteCount;
            file.read(reinterpret_cast<char*>(&attributeByteCount), sizeof(attributeByteCount));
        }

        return std::make_pair(vertices, triangles);
    };

    // Lambda to read ASCII STL files
    auto readAsciiSTL = [&file, &arrayHash, &meshName]() {
        std::vector<std::array<double, 3>> vertices;
        std::vector<std::array<unsigned int, 3>> triangles;
        std::unordered_map<std::array<double, 3>, unsigned int, decltype(arrayHash)> vertexMap(0, arrayHash);

        std::string line;
        std::array<unsigned int, 3> currentTriangle;
        int vertexIndex = 0;

        bool firstLine = true;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string keyword;
            iss >> keyword;

            if (firstLine && keyword == "solid") {
                // Extract the mesh name after "solid" on the first line
                iss >> meshName;
                firstLine = false;
            } else if (keyword == "vertex") {
                std::array<double, 3> vertex;
                iss >> vertex[0] >> vertex[1] >> vertex[2];

                auto it = vertexMap.find(vertex);
                if (it == vertexMap.end()) {
                    unsigned int index = vertices.size();
                    vertices.push_back(vertex);
                    vertexMap[vertex] = index;
                    currentTriangle[vertexIndex] = index;
                } else {
                    currentTriangle[vertexIndex] = it->second;
                }

                vertexIndex = (vertexIndex + 1) % 3;
                if (vertexIndex == 0) {
                    triangles.push_back(currentTriangle);
                }
            }
        }

        return std::make_pair(vertices, triangles);
    };

    // Check if the file is binary or ASCII
    char header[5];
    file.read(header, 5);
    file.seekg(0, std::ios::beg); // Reset file position

    std::pair<std::vector<std::array<double, 3>>, std::vector<std::array<unsigned int, 3>>> rawResult;
    if (std::string(header, 5) == "solid") {
        // ASCII STL
        rawResult = readAsciiSTL();
    } else {
        // Binary STL
        rawResult = readBinarySTL();
    }

    // Convert the result to sLib3MFPosition and sLib3MFTriangle
    std::vector<sLib3MFPosition> positions;
    for (const auto& vertex : rawResult.first) {
        positions.push_back(fnCreateVertex(static_cast<float>(vertex[0]), static_cast<float>(vertex[1]), static_cast<float>(vertex[2])));
    }

    std::vector<sLib3MFTriangle> triangles;
    for (const auto& triangle : rawResult.second) {
        triangles.push_back(fnCreateTriangle(static_cast<int>(triangle[0]), static_cast<int>(triangle[1]), static_cast<int>(triangle[2])));
    }

    return std::make_tuple(meshName, positions, triangles);
}

// Structure to store mesh data
struct MeshData {
    std::vector<std::vector<float>> vertices;  // Each vertex has x, y, z
    std::vector<std::vector<int>> triangles;   // Each triangle has three indices

    // Conversion function to return JavaScript-friendly format
    emscripten::val toJS() const {
        emscripten::val verticesJS = emscripten::val::array();
        emscripten::val trianglesJS = emscripten::val::array();

        // Convert vertices to JavaScript array format
        for (const auto& vertex : vertices) {
            emscripten::val vertexJS = emscripten::val::array();
            for (const auto& coord : vertex) {
                vertexJS.call<void>("push", coord);
            }
            verticesJS.call<void>("push", vertexJS);
        }

        // Convert triangles to JavaScript array format
        for (const auto& triangle : triangles) {
            emscripten::val triangleJS = emscripten::val::array();
            for (const auto& index : triangle) {
                triangleJS.call<void>("push", index);
            }
            trianglesJS.call<void>("push", triangleJS);
        }

        // Return as JavaScript object with `vertices` and `triangles` fields
        emscripten::val jsObject = emscripten::val::object();
        jsObject.set("vertices", verticesJS);
        jsObject.set("triangles", trianglesJS);

        return jsObject;
    }
};

// Function that creates a 3MF cube model
void createCube(const std::string& outputFile) {
    try {
        PWrapper wrapper = CWrapper::loadLibrary();
        PModel model = wrapper->CreateModel();

        // Create a mesh object (cube)
        PMeshObject meshObject = model->AddMeshObject();
        meshObject->SetName("Box");

        // Define cube dimensions
        float fSizeX = 100.0f;
        float fSizeY = 200.0f;
        float fSizeZ = 300.0f;

        // Create vertices for a cube
        std::vector<sLib3MFPosition> vertices(8);
        vertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
        vertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
        vertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
        vertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
        vertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
        vertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
        vertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
        vertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);

        // Create triangles for a cube
        std::vector<sLib3MFTriangle> triangles(12);
        triangles[0] = fnCreateTriangle(2, 1, 0);
        triangles[1] = fnCreateTriangle(0, 3, 2);
        triangles[2] = fnCreateTriangle(4, 5, 6);
        triangles[3] = fnCreateTriangle(6, 7, 4);
        triangles[4] = fnCreateTriangle(0, 1, 5);
        triangles[5] = fnCreateTriangle(5, 4, 0);
        triangles[6] = fnCreateTriangle(2, 3, 7);
        triangles[7] = fnCreateTriangle(7, 6, 2);
        triangles[8] = fnCreateTriangle(1, 2, 6);
        triangles[9] = fnCreateTriangle(6, 5, 1);
        triangles[10] = fnCreateTriangle(3, 0, 4);
        triangles[11] = fnCreateTriangle(4, 7, 3);

        // Set the cube geometry
        meshObject->SetGeometry(vertices, triangles);

        // Add the cube as a build item
        model->AddBuildItem(meshObject.get(), wrapper->GetIdentityTransform());

        // Write the cube model to a 3MF file in the virtual filesystem
        PWriter writer = model->QueryWriter("3mf");
        writer->WriteToFile(outputFile);

        std::cout << "Cube creation completed: " << outputFile << std::endl;
    } catch (ELib3MFException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Function that retrieves mesh data from a 3MF file
MeshData getMeshData(const std::string& inputFile) {
    MeshData meshData;
    try {
        PWrapper wrapper = CWrapper::loadLibrary();
        PModel model = wrapper->CreateModel();
        PReader reader = model->QueryReader("3mf");
        reader->ReadFromFile(inputFile);

        const auto objectIterator = model->GetObjects();
        while (objectIterator->MoveNext()) {
            const auto currentObject = objectIterator->GetCurrentObject();
            if (currentObject->IsMeshObject()) {
                const auto currentMesh = model->GetMeshObjectByID(currentObject->GetResourceID());

                for (size_t v = 0; v < currentMesh->GetVertexCount(); v++) {
                    auto vertex = currentMesh->GetVertex(v);
                    meshData.vertices.push_back({
                        vertex.m_Coordinates[0],
                        vertex.m_Coordinates[1],
                        vertex.m_Coordinates[2]
                    });
                }

                for (size_t t = 0; t < currentMesh->GetTriangleCount(); t++) {
                    auto triangle = currentMesh->GetTriangle(t);
                    meshData.triangles.push_back({
                        static_cast<int>(triangle.m_Indices[0]),
                        static_cast<int>(triangle.m_Indices[1]),
                        static_cast<int>(triangle.m_Indices[2])
                    });
                }
            }
        }
    } catch (ELib3MFException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return meshData;
}

int multiSTLTo3MF(emscripten::val inputFiles, emscripten::val inputColors, emscripten::val inputNames, const std::string& outputFile) {
    try {
        // Convert JavaScript arrays to std::vector
        std::vector<std::string> fileNames;
        std::vector<std::tuple<int, int, int, int>> colors;
        std::vector<std::string> meshNames;

        const unsigned int fileLength = inputFiles["length"].as<unsigned int>();
        for (unsigned int i = 0; i < fileLength; ++i) {
            fileNames.push_back(inputFiles[i].as<std::string>());
        }

        const unsigned int colorLength = inputColors["length"].as<unsigned int>();
        for (unsigned int i = 0; i < colorLength; ++i) {
            auto colorObj = inputColors[i];
            colors.emplace_back(
                colorObj["r"].as<int>(),
                colorObj["g"].as<int>(),
                colorObj["b"].as<int>(),
                colorObj["a"].as<int>()
            );
        }

        const unsigned int nameLength = inputNames["length"].as<unsigned int>();
        for (unsigned int i = 0; i < nameLength; ++i) {
            meshNames.push_back(inputNames[i].as<std::string>());
        }

        // Create a 3MF wrapper and model
        PWrapper wrapper = CWrapper::loadLibrary();
        PModel model = wrapper->CreateModel();

        // Create a single Color Group for the model
        PColorGroup colorGroup = model->AddColorGroup();

        for (size_t i = 0; i < fileNames.size(); ++i) {
            // Read STL file
            std::string meshName;
            std::vector<sLib3MFPosition> positions;
            std::vector<sLib3MFTriangle> triangles;
            std::tie(meshName, positions, triangles) = readSTL(fileNames[i]);

            std::cout << "Processing file: " << fileNames[i] << std::endl;
            std::cout << "  Mesh Name (from file): " << meshName << std::endl;
            std::cout << "  Vertices: " << positions.size() << std::endl;
            std::cout << "  Triangles: " << triangles.size() << std::endl;

            // Use provided name if available, otherwise fallback to the filename
            std::string finalMeshName = (i < meshNames.size()) ? meshNames[i] : meshName;

            // Create a mesh object and set geometry
            Lib3MF::PMeshObject meshObject = model->AddMeshObject();
            meshObject->SetName(finalMeshName);  // ✅ Set user-provided mesh name
            meshObject->SetGeometry(positions, triangles);

            // Assign color if available
            if (i < colors.size()) {
                auto [r, g, b, a] = colors[i];
                Lib3MF_uint32 colorID = colorGroup->AddColor(wrapper->RGBAToColor(r, g, b, a));

                // **FIX: Assign object-level property**
                meshObject->SetObjectLevelProperty(colorGroup->GetResourceID(), colorID);

                // Set per-triangle color properties
                sLib3MFTriangleProperties meshColor = fnCreateTriangleColor(colorGroup, colorID, colorID, colorID);
                for (size_t t = 0; t < triangles.size(); ++t) {
                    meshObject->SetTriangleProperties(t, meshColor);
                }
            }

            // Add mesh object to build
            model->AddBuildItem(meshObject.get(), createTranslationMatrix(0.0f, 0.0f, 0.0f));
        }

        // Export to 3MF
        Lib3MF::PWriter writer = model->QueryWriter("3mf");
        std::cout << "Writing 3MF file to: " << outputFile << std::endl;
        writer->WriteToFile(outputFile);

        std::cout << "3MF file successfully created!" << std::endl;
        return 0; // Success
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return -1; // Error
    }
}


// Function that converts a file between 3MF and STL formats
int convert(const std::string& inputFile, const std::string& outputFile) {
    try {
        PWrapper wrapper = CWrapper::loadLibrary();
        PModel model = wrapper->CreateModel();
        PReader reader = model->QueryReader("3mf");
        reader->ReadFromFile(inputFile);
        PWriter writer = model->QueryWriter("stl");
        writer->WriteToFile(outputFile);

        std::cout << "Conversion completed: " << inputFile << " to " << outputFile << std::endl;
        return 0;
    } catch (ELib3MFException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return e.getErrorCode();
    }
}

emscripten::val validate3MF(const std::string& inputFile) {
    emscripten::val result = emscripten::val::object();
    emscripten::val warnings = emscripten::val::array();

    try {
        PWrapper wrapper = CWrapper::loadLibrary();
        PModel model = wrapper->CreateModel();
        PReader reader = model->QueryReader("3mf");

        reader->SetStrictModeActive(false);
        reader->ReadFromFile(inputFile);

        for (Lib3MF_uint32 iWarning = 0; iWarning < reader->GetWarningCount(); iWarning++) {
            Lib3MF_uint32 nErrorCode;
            std::string sWarningMessage = reader->GetWarning(iWarning, nErrorCode);
            emscripten::val warning = emscripten::val::object();
            warning.set("code", nErrorCode);
            warning.set("message", sWarningMessage);
            warnings.call<void>("push", warning);
        }

        result.set("status", "success");
        result.set("warnings", warnings);
        result.set("message", "Validation completed successfully.");
    } catch (...) {
        result.set("status", "error");
        result.set("message", "An unknown exception occurred.");
    }

    return result;
}




// Bindings for JavaScript access
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::register_vector<std::string>("VectorString");
    emscripten::function("createCube", &createCube);
    emscripten::function("convertFile", &convert);
    emscripten::function("stlTo3MF", &multiSTLTo3MF);
    emscripten::function("getMeshData", &getMeshData);
    emscripten::function("validate3MF", &validate3MF);
    emscripten::class_<MeshData>("MeshData")
        .function("toJS", &MeshData::toJS);
}
