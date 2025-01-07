file(GLOB sources *.cpp)
add_library(parse ${sources})
target_include_directories(parse INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
