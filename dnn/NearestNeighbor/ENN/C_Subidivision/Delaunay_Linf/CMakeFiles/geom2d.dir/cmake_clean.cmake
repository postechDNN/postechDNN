file(REMOVE_RECURSE
  "libgeom2d.a"
  "libgeom2d.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/geom2d.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
