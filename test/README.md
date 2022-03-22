# Testing

Triangles project has 2 types of tests: unit tests and end-to-end tests.

## Unit tests

Based on [GTest](https://google.github.io/googletest/). How to run all unit tests:

```bash
$ ctest -VV -L unit
```

Also you may run every test individually:

```bash
$ /path/to/Triangles/build/bin/test/testname_tst
```

## End-to-end tests

Based on [lit](https://llvm.org/docs/CommandGuide/lit.html).
Useful article [here](https://medium.com/@mshockwave/using-llvm-lit-out-of-tree-5cddada85a78).
How to run:

```bash
$ # 1st way
$ ctest -VV -L e2e
$
$ # 2nd way
$ python3 /path/to/Triangles/test/e2e/my-lit.py \
          /path/to/Triangles/build/test/e2e -vv
```
