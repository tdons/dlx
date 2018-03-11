libdlx
======

libdlx is a C implementation of *dancing links*, an efficient backtracking algorithm that solves
instances of the Exact Cover problem.  For more information on the Exact Cover problem see the
paper in `libdlx/doc/` or visit [wikipedia](https://en.wikipedia.org/wiki/Exact_cover)

This project includes two non-trivial example programs that use libdlx: sudgen and sudslv.
sudgen is a sudoku puzzle generator, sudslv is a sudoku puzzle solver.


building
--------

```
$ make
```

The build will produce these artifacts:
- `libdlx` (both a static and a dynamic library)
- `sudgen` (sudoku puzzle generator)
- `sudslv` (sudoku puzzle solver)


Cross compile to js using `$ make js` (you need to have Emscripten installed).
Run afterwards using `$ node build-js/bin/sudgen.js`


Run `$ make test` after building or try the below command (your output will differ, puzzle is randomly generated):

```shell
$ pushd build && ./bin/sudgen | tee >(./bin/sudslv) | ../sudoku/pretty-print-puzzle.py && popd
+---+---+---+
|5 3|   |8 7|
|   |   |   |
|   | 76|4  |
+---+---+---+
|1  | 3 |   |
|   |9 5|  1|
|  2| 4 |   |
+---+---+---+
|74 |   |3  |
| 5 |  1| 6 |
|  8|   | 2 |
+---+---+---+
+---+---+---+
|563|412|897|
|427|893|156|
|981|576|432|
+---+---+---+
|175|638|249|
|834|925|671|
|692|147|583|
+---+---+---+
|746|259|318|
|259|381|764|
|318|764|925|
+---+---+---+
```

Tested under:

- OSX 10.11 using clang 7.0.0
- FreeBSD 9.3 using cc 4.2.1

project layout
--------------

- `libdlx/`

   C library, implementation of *dancing links*

- `sudoku/sudgen/`
  
  Sudoku generator.  Outputs a single puzzle to stdout.
  
  Properties of generated puzzles:
  - there is a unique solution
  - puzzle is minimal, that is to say that removing any given would render it non-unique

- `sudoku/sudslv/`

   Reads sudoku puzzles (1 per line) from stdin until EOF and:
   - outputs their solution as a single line if there is one
   - reports to stderr if the puzzle has 0 or more than 1 solution

- `pydlx/`

  Python bindings, these are needed for running the unit tests and most of the examples.


dependencies
------------

None except the C standard library.

Note: you do need python to run the tests and most of the examples.


using libdlx
------------

A simple example of how to use `libdlx` can be found in `examples/example-2x2-matrix.c`.

In addition, the examples directory contains scripts to generate instances to Exact Cover with, these include:

- `examples/generate-nxn-latin-square-matrix.py n`

   count latin squares of size `n`
   
- `examples/generate-n-queens-matrix.py n`

   solving the `n`-queens problem (n=19 took about 3.5hrs)
   
- `examples/generate-nxn-sudoku-matrix.py n`

   counting sudoku grids with boxes of size `n`

To solve the 8-queens problem you'd run this:
```shell
$ cd examples
$ ./generate-n-queens-matrix.py 8 | ./count-solutions.py
92
```

speed
-----

The implementation is pretty speedy:

```shell
$ time xz -dc sudoku/sudoku17.txt.xz | ./build/bin/sudslv > /dev/null
xz -dc sudoku/sudoku17.txt.xz  0.04s user 0.01s system 1% cpu 2.821 total
./build/bin/sudslv > /dev/null  2.87s user 0.00s system 99% cpu 2.880 total

$ xz -dc sudoku/sudoku17.txt.xz | wc -l
   49151 data/sudoku17.txt
```

`sudslv` can solve about 17,125 puzzles/sec/core on a 3.5 GHz Intel Core i5

