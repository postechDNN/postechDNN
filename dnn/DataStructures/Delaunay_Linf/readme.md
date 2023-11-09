Delaunay triangulation with L_inf

# How to use CMake

1. Install cmake
2. run
   ```
   cmake -DCMAKE_OSX_ARCHITECTURES=arm64 CMakeLists.txt 
   ```

    if you are using arm64 os.

Then it will make a Makefile. 

If you run `make`, you will have `delaunay`. 

Then, run `delaunay`.
