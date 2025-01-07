file(GLOB sources *.cpp)
add_library(corr ${sources})
target_include_directories(corr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(corr PUBLIC compiler_flags)
