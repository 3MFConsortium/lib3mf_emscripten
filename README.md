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

There is also a `index.html` file inside `assets` folder which goes
together with the generated `.js` and `.wasm`

In order to test, this file needs to be copied to build directory and 
then do the following

```shell
python -m http.server
```

This will launch a local webserver in your machine