# Garbage Collector Examples (oxy)

This repository contains small C implementations demonstrating two different
garbage-collection strategies for a tiny object system named "oxy":

- a reference-counted collector (in `reference_counted_gc/`)
- a mark-and-sweep collector (in `mark_and_sweep_gc/`)

It also includes a tiny dynamic `stack` helper and a bundled copy of
the `µnit` test framework under `munit/` (used by the reference-counted tests).

Use this README as a quick reference for the project layout, how to build
the provided examples/tests, and known issues I discovered while reading the code.

Overview
--------
You can explore and study both GC strategies in this project. Each strategy
implements a minimal set of object types (integers, floats, strings,
3-element vectors, and arrays) along with operations and memory-management
helpers.

Project structure
-----------------
Top-level layout (relevant files and folders):

- `Makefile` - convenience target to run the reference-counted test.
- `munit/` - bundled micro unit testing framework (MIT-licensed).
  - `munit.c`, `munit.h`, `Makefile`, etc.
- `stack/` - a small dynamic stack for use by VM/frames and object tracking.
  - `stack.h`, `stack.c`
- `reference_counted_gc/` - reference-counted object implementation + a test.
  - `oxy_object.h`, `oxy_object.c`, `test.c`
- `mark_and_sweep_gc/` - mark-and-sweep GC implementation + supporting files.
  - `oxy_object.h`, `oxy_object.c`, `oxy_utils.h`, `vm.h`, `vm.c`, `test.c` (placeholder)

High-level responsibilities:
- `oxy_object.*` files define object shapes and operations.
- `vm.*` (mark-and-sweep) implements marking, tracing and sweeping + VM object/frame bookkeeping.
- `stack/` provides a growable container used to store tracked objects and
  frame references.
- `munit/` is used to run unit tests (reference-counted test uses this).

Build and run
-------------

- Run the provided reference-counted test via the Makefile:

```garbage-collector/README.md#L1-6
make test_ref
```

This runs the recipe that compiles `reference_counted_gc/test.c` together with
`reference_counted_gc/oxy_object.c` and `munit/munit.c`, and then executes the
resulting test binary.

- Manual compile example (reference-counted test):

```garbage-collector/README.md#L7-13
gcc -std=c11 reference_counted_gc/test.c reference_counted_gc/oxy_object.c \
    munit/munit.c -I. -o reference_counted_gc/test_ref && \
    ./reference_counted_gc/test_ref
```

- Manual compile example (mark-and-sweep example):
  There is not a working test binary committed for `mark_and_sweep_gc/` but
  you can build a small runtime to experiment by compiling `vm.c`, the
  `mark_and_sweep_gc` object files and the `stack` helper. Example:

```garbage-collector/README.md#L14-20
gcc -std=c11 -I. \
    mark_and_sweep_gc/vm.c \
    mark_and_sweep_gc/oxy_object.c \
    stack/stack.c \
    -o mark_and_sweep_example
./mark_and_sweep_example
```

Depending on which example you compile you may need to add or remove source
files and/or write a small `main()` to exercise the VM. The `mark_and_sweep_gc/test.c`
file appears present but empty in this checkout, so there is no ready-to-run
test binary for the mark-and-sweep example.

Running a single test source file with `munit`:
If you want to add tests that use the bundled `munit`, include `munit/munit.c`
on the link line and include `munit/munit.h` in your test source.

Design notes
------------
- Object model: ints, floats, strings, 3-element vectors and arrays.
- Reference counting: increments/decrements reference counts and frees
  recursively when count reaches zero.
- Mark-and-sweep: VM tracks objects and frames; GC phases are `mark`, `trace`,
  and `sweep`.

Contributing
------------
If you plan to contribute changes:
- Follow the existing C coding style in the repository.
- Add targeted unit tests under `reference_counted_gc/` or a new test under
  `mark_and_sweep_gc/` using the included `munit` harness.

License
-------
- `munit/` is MIT-licensed (see `munit/COPYING`).
- The rest of the project does not contain an explicit top-level license file
  in this checkout. Add a LICENSE file if you intend to open-source this work.