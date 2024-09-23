cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)

add_executable(delaunay ./externals/quadedge-basic/geom2d.h ./externals/quadedge-basic/quadedge.cpp ./externals/quadedge-basic/quadedge.h main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


find_package(Boost COMPONENTS filesystem system iostreams REQUIRED)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/externals/gnuplot-iostream")


include_directories(${OPENGL_INCLUDE_DIRS}  ${GL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

target_link_libraries(delaunay ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${Boost_LIBRARIES})

