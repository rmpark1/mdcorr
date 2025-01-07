file(GLOB sources *.cpp)
add_library(array ${sources})
target_include_directories(array INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
