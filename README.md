# Triangles

3D geometry library.

## Build

To build and test Triangle project you have to install several packages: `libgtest-dev`.

```bash
$ sudo apt-get install libgtest-dev
```

After that you have to configure `cmake`. You are able to set any compiler wich support C++20 concepts, any build type, turn on/off documentation build or set generator:

```bash
$ cd /path/to/Triangles
$ mkdir build & cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release \
           -DCMAKE_CXX_COMPILER=clang++-10 \
           -DBUILD_DOC=OFF
```

Then build project:

```bash
$ cmake --build . -j`nproc`
```

All the executables will be placed to `/path/to/Triangles/build/bin`.

## Run tests

To run tests you have to install `llvm-10-tools` package and python library named `lit`:

```bash
$ sudo apt-get install llvm-10-tools
$ pip3 install lit
```

Run all tests with command:

```bash
$ cd /path/to/Triangles/build
$ ctest -VV
```
