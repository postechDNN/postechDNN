cmake_minimum_required(VERSION 3.2)

project(delaunay_lib VERSION 1.0.1 DESCRIPTION "Making a library including Delaunay_Linf")

set(CMAKE_CXX_STANDARD 17)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Boost COMPONENTS filesystem system iostreams REQUIRED)

add_library(quadedge SHARED 
    ./../externals/quadedge-basic/quadedge.cpp   
    ./../externals/quadedge-basic/quadedge.h
    )

target_link_libraries(quadedge ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${Boost_LIBRARIES})

add_library(delaunay_lib STATIC 
    ./../delaunay.h 
    ./../externals/quadedge-basic/fakeirisgl.cpp 
    ./../externals/quadedge-basic/geom2d.h    
    ./../externals/quadedge-basic/fakeirisgl.h 
)

set_target_properties(delaunay_lib PROPERTIES PREFIX "")
set_target_properties(delaunay_lib PROPERTIES OUTPUT_NAME "delaunay_lib")

target_link_libraries(delaunay_lib quadedge ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${Boost_LIBRARIES})
