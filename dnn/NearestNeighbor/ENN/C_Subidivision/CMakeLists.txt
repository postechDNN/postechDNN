cmake_minimum_required(VERSION 3.2)

project(test_sub VERSION 1.0.1 DESCRIPTION "Test C_subdivision")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Delaunay_Linf/making_lib)

add_library(dcel "")
set(DCEL_DIR ${CMAKE_CURRENT_LIST_DIR}/DCEL)
target_sources(dcel 
    PUBLIC 
    ${DCEL_DIR}/DCEL_IO.cpp
    ${DCEL_DIR}/DCEL.cpp
    ${DCEL_DIR}/DCEL_operation.cpp 
    ${DCEL_DIR}/DCEL.h
    ${DCEL_DIR}/Vector.cpp 
    ${DCEL_DIR}/Vector.h 
    ${DCEL_DIR}/Edge.cpp 
    ${DCEL_DIR}/Edge.h
    ${DCEL_DIR}/Point.cpp 
    ${DCEL_DIR}/Point.h
    ${DCEL_DIR}/Polygon.cpp 
    ${DCEL_DIR}/Polygon.h
    )
target_include_directories(dcel PUBLIC ${CMAKE_CURRENT_LIST_DIR}/DCEL)



add_library(wcregion "")
target_sources(wcregion 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/WC_region.cpp
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/WC_region.h
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/DCEL/DCEL.h
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/DCEL/Vector.h
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/DCEL/Point.h)
target_link_libraries(wcregion dcel)
target_include_directories(wcregion
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/DCEL
  )

add_library(test_lib "")
set(TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/Testcode/test_csdiv_pts)
target_sources(test_lib
    PUBLIC 
    ${TEST_DIR}/test_csdiv_pts.cpp 
    ${TEST_DIR}/test_csdiv_pts.h
    ${TEST_DIR}/../../DCEL/Point.h 
    ${TEST_DIR}/../../C_Subdivision.h 
    ${TEST_DIR}/CS_valid_check.cpp
    ${TEST_DIR}/CS_valid_check.h
    ${TEST_DIR}/../../WC_region.h
    ${TEST_DIR}/../../DCEL/DCEL.h
)
target_link_libraries(test_lib dcel wcregion)
target_include_directories(test_lib PUBLIC
${CMAKE_CURRENT_LIST_DIR}/DCEL
${CMAKE_CURRENT_LIST_DIR})
  

add_library(csub "")
target_sources(csub
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/C_subdivision.cpp
    ${CMAKE_CURRENT_LIST_DIR}/C_subdivision.h)
target_link_libraries(csub dcel test_lib wcregion)
target_include_directories(csub PUBLIC ${CMAKE_CURRENT_LIST_DIR}/DCEL ${CMAKE_CURRENT_LIST_DIR}/Testcode/test_csdiv_pts)

add_executable(test_sub main.cpp)
target_link_libraries(test_sub csub dcel wcregion test_lib delaunay_lib)



