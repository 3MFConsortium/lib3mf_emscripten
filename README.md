lib3mf emscripten demos
=======================

This repository holds the code for lib3mf emscripten demos

This will not build on anything other than ubuntu 24.04.
It is only kept here for archival purposes. 

It has the following demos

* Create Cube
* File conversion
* 3MF Validator
* Multiple STL to 3MF (With optional color chooser)


If you have a Ubuntu 24.04 machine, you can do the following

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