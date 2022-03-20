# Triangles

3D geometry library.

![GitHub repo size](https://img.shields.io/github/repo-size/106-inc/Triangles?style=for-the-badge)
![GitHub last commit](https://img.shields.io/github/last-commit/106-inc/Triangles?color=red&style=for-the-badge)
![GitHub lines](https://img.shields.io/tokei/lines/github/106-inc/Triangles?style=for-the-badge)

## Building

To build and test Triangle project you have to install several packages: `libgtest-dev`.

```bash
$ sudo apt-get install libgtest-dev
```

After that you have to configure `cmake`. You are able to set any compiler which supports C++20 concepts, any build type, turn on/off documentation build or set generator:

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

All the executables will be placed in `/path/to/Triangles/build/bin`.

## Running tests

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
