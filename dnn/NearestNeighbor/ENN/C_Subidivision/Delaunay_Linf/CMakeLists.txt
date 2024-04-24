cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(CMAKE_GENERATOR "Unix Makefiles")

project(delaunay VERSION 1.0.1 DESCRIPTION "Delaunay_Linf")


add_library(geom2d "")
set_target_properties(geom2d PROPERTIES LINKER_LANGUAGE CXX)
target_sources(geom2d PUBLIC 
    ${EXT_DIR}/quadedge-basic/geom2d.h
)

add_library(quadedge "")
target_sources (quadedge PUBLIC
    ${EXT_DIR}/quadedge-basic/quadedge.cpp
    ${EXT_DIR}/quadedge-basic/quadedge.h
)
target_include_directories(quadedge PUBLIC 
    ${EXT_DIR}/quadedge-basic
)

add_library(delaunay STATIC)
set_target_properties(delaunay PROPERTIES LINKER_LANGUAGE CXX)
target_sources(delaunay PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/delaunay.h
    ${EXT_DIR}/quadedge-basic/fakeirisgl.cpp 
    ${EXT_DIR}/quadedge-basic/geom2d.h 
    ${EXT_DIR}/quadedge-basic/fakeirisgl.h)  
target_include_directories(delaunay PUBLIC 
    ${EXT_DIR}/quadedge-basic
)
target_link_libraries(delaunay quadedge geom2d )


add_executable(delaunay_test main.cpp)
set_target_properties(delaunay_test PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(delaunay_test delaunay quadedge geom2d)
target_include_directories(delaunay_test PUBLIC 
    ${EXT_DIR}/gnuplot-iostream
    ${EXT_DIR}/quadedge-basic
    )




