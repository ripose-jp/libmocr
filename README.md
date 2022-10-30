# libmocr

A library providing C and C++ bindings for [mangaocr](https://github.com/kha-white/manga-ocr)

# Building

## Dependencies

* Python
* A C99 compatible compiler
* A C++11 compatible compiler

## Building

Make sure CMake is installed, navigate to the root of the repository, and
run the following:
```
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
The libraries should be in the `build` folder.

## Installing (Linux)

Navigate to the build folder and run:
```
sudo cmake --install . --config Release
sudo ldconfig /usr/local/lib
```

## Compiling Projects with libmocr

This section assume you are on Linux and the dependencies are installed to a
known location such as `/usr/local/lib`.

Compiling with the C API:
```
gcc example.c -lmocr -o example
```

Compiling with the C++ API:
```
g++ example.cpp -lmocr++ -o example
```

## Notes

libmocr **does not** bundle mangaocr.
The mangaocr Python library **must** be available to Python at runtime just like
any other Python library.

mangaocr can be installed from `pip` by running:
```
pip3 install manga-ocr
```

# Usage

Below are simple programs that read in an image file from the command line and
prints out the extracted text to stdout.

## C

```c
#include <stdio.h>
#include <mocr.h>

int main(int argc, char **argv)
{
    if (argc != 2) return 1;
    mocr_ctx *ctx = mocr_init(DEFAULT_MODEL, 0);
    char *text = mocr_read_file(ctx, argv[1]);
    puts(text);
    mocr_free(text);
    mocr_destroy(ctx);
}
```

## C++

```cpp
#include <iostream>
#include <mocr++.h>

int main(int argc, char **argv)
{
    if (argc != 2) return 1;
    mocr::model mod;
    if (!mod) return 1;
    std::cout << mod.read(argv[1]) << std::endl;
}
```

# Troubleshooting

## ModuleNotFoundError: No module named 'manga_ocr'

Make sure mangaocr is installed by running
```
pip3 install manga-ocr
```

If you have multiple Python versions installed, it's possible that libmocr is
linked to a version that doesn't have mangaocr installed.

Run `pip3 --version` to find the Python version connected to `pip`.
Recompile libmocr with the `-DEXACT_PYTHON_VERSION=<version>` option set to the
same version as the Python version number reported by `pip`.

This can be done by running in the build directory:
```
cmake -DEXACT_PYTHON_VERSION=3.10 ..
```

# Acknowledgements

Thanks to [kha-white](https://github.com/kha-white) and the other mangaocr
contributors for creating this library, the model, and compiling the test data.

Testing done with [GoogleTest](https://github.com/google/googletest).

The [stb project](https://github.com/nothings/stb) for the
[stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) library
used for testing.
