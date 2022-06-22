cmake_minimum_required(VERSION 3.2)


add_executable(delaunay geom2d.h quadedge.cpp quadedge.h main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


include_directories(${OPENGL_INCLUDE_DIRS}  ${GL_INCLUDE_DIRS})

target_link_libraries(delaunay ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

