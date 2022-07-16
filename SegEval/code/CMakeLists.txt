cmake_minimum_required(VERSION 2.8)
 
project(testing)
 
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

 
add_executable(testing MACOSX_BUNDLE testing.cxx)
target_link_libraries(testing ${ITK_LIBRARIES})
