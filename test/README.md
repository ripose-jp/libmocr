# Testing

Tests are not enabled by default. They must be explicitly requested at configure
time.

# Building

Navigate to the source directory and run
```
mkdir build
cd build
cmake -DBUILD_TESTING=ON ..
cmake --build .
ctest
```

# Warning

Do not run tests with a ton of jobs.
Python requires a global interpreter lock, so there is almost no speed up from
running tests in parallel.
Rather, running tests with a ton of jobs is likely to cause the host to run out
of memory due to loading the ML model multiple times in memory.
