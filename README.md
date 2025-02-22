lib3mf emscripten demos
=======================

This repository holds the code for lib3mf emscripten demos

This will only build on Linux. 
It is only kept here for archival purposes. 

It has the following demos

* Create Cube
* File conversion
* 3MF Validator
* Multiple STL to 3MF (With optional color chooser)


If you have a Linux machine, you can do the following

First make sure to pull vcpkg

```shell
git submodule update --init --recursive
```

Also ensure that `emcc` and `em++` are installed and available in system path

Then do the following

```shell
mkdir build
cd build
bash ../build.sh
```

The output should be something like this

```shell
12:01:56  |base|vijai@curie build ±|main|→ bash ../build.sh 
../build.sh: line 1: ../vcpkg/vcpkg/bootstrap-vcpkg.sh: Not a directory
Computing installation plan...
warning: lib3mf is only supported on '(windows & (x86 | x64) & !static & !staticcrt) | (linux & x64) | (osx & (x64 | arm64))', which does not match wasm32-emscripten. This usually means that there are known build failures, or runtime problems, when building other platforms. Proceeding anyway due to `--allow-unsupported`.
The following packages are already installed:
    lib3mf:wasm32-emscripten@2.3.2
lib3mf:wasm32-emscripten is already installed
Total install time: 301 us
lib3mf provides CMake targets:

  find_package(lib3mf CONFIG REQUIRED)
  target_link_libraries(target_name PRIVATE lib3mf::lib3mf) 

For examples and use cases, kindly check
https://github.com/3MFConsortium/lib3mf/tree/develop/SDK/CPackExamples

-- The C compiler identification is Clang 20.0.0
-- The CXX compiler identification is Clang 20.0.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - failed
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - failed
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found ZLIB: optimized;/home/vijai/Code/3MF/lib3mf_emscirpten_demo/vcpkg/installed/wasm32-emscripten/lib/libz.a;debug;/home/vijai/Code/3MF/lib3mf_emscirpten_demo/vcpkg/installed/wasm32-emscripten/debug/lib/libz.a (found suitable version "1.3.1", minimum required is "1.1.2")
-- Found BZip2: optimized;/home/vijai/Code/3MF/lib3mf_emscirpten_demo/vcpkg/installed/wasm32-emscripten/lib/libbz2.a;debug;/home/vijai/Code/3MF/lib3mf_emscirpten_demo/vcpkg/installed/wasm32-emscripten/debug/lib/libbz2d.a (found version "1.0.8")
-- Looking for BZ2_bzCompressInit
-- Looking for BZ2_bzCompressInit - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE
-- Found OpenSSL: /home/vijai/Code/3MF/lib3mf_emscirpten_demo/vcpkg/installed/wasm32-emscripten/debug/lib/libcrypto.a (found version "3.4.1")
-- Found ZLIB: optimized;/home/vijai/Code/3MF/lib3mf_emscirpten_demo/vcpkg/installed/wasm32-emscripten/lib/libz.a;debug;/home/vijai/Code/3MF/lib3mf_emscirpten_demo/vcpkg/installed/wasm32-emscripten/debug/lib/libz.a (found version "1.3.1")
Lib3MF - VCPKG Tool Chain
***********************************
LIB3MF Chosen Variant : Cpp
***********************************
-- Configured lib3mfEmscriptenTest for Emscripten using VCPKG.
-- Configuring done (4.4s)
-- Generating done (0.0s)
-- Build files have been written to: /home/vijai/Code/3MF/lib3mf_emscirpten_demo/build
[ 50%] Building CXX object CMakeFiles/lib3mfEmscriptenTest.js.dir/main.cpp.o
em++: warning: linker setting ignored during compilation: 'ASSERTIONS' [-Wunused-command-line-argument]
em++: warning: linker setting ignored during compilation: 'ALLOW_MEMORY_GROWTH' [-Wunused-command-line-argument]
em++: warning: DISABLE_EXCEPTION_CATCHING=X is no longer needed when specifying EXCEPTION_CATCHING_ALLOWED [-Wdeprecated]
[100%] Linking CXX executable lib3mfEmscriptenTest.js
em++: warning: DISABLE_EXCEPTION_CATCHING=X is no longer needed when specifying EXCEPTION_CATCHING_ALLOWED [-Wdeprecated]
[100%] Built target lib3mfEmscriptenTest.js
12:02:17  |base|vijai@curie build ±|main|→ 
```

There is also a `index.html` file inside `assets` folder which goes
together with the generated `.js` and `.wasm`

In order to test, this file needs to be copied to build directory and 
then do the following

```shell
python -m http.server
```

This will launch a local webserver in your machine