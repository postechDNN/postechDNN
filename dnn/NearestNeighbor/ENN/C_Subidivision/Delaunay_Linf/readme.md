Delaunay triangulation with L_inf

# How to use CMake

1. Install cmake
2. run
   2.1. Window

   ```
   cmake -G "Unix Makefiles" 
   ```

   2.2. Mac

   ```
   cmake -DCMAKE_OSX_ARCHITECTURES=arm64 CMakeLists.txt 
   ```

Then it will make a Makefile.

If you run `make`, you will have `delaunay_test.exe` (or  `delaunay_test`).

Then, run `delaunay_test`.
