cmake_minimum_required(VERSION 2.8)
project(newyear_interval)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(./src srcfiles)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(newyear_interval ${srcfiles})
target_link_libraries(newyear_interval -lWSTP64i4 -lm -lpthread -lrt -lstdc++ -ldl -luuid -lcln -lginac -ljsoncpp)
