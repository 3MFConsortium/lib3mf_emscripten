lib3mf emscripten bindings
===========================

This repository holds the code for lib3mf emscripten bindings

It builds on both windows and linux

It has the following demos

* Create Cube
* File conversion
* Color cube
* Beam lattice
* 3mf file info
* Add triangle
* 3mf Components

and a lot more to come  


There is a custom build script. Once you have emscripten installed based on the instructions laid out here https://emscripten.org/docs/getting_started/downloads.html

You just need to do the following

```shell
python lib3mf_emscripten_bindings_build.py --build-type=Release --out-dir=build_bindings 
```

You can include the `--verbose` flag if you want

This should produce the `.js` and `.wasm` files in the output directory and you can then run the examples from the examples directory
