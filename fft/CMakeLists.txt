file(GLOB sources *.cpp)
add_library(fft ${sources})
target_include_directories(fft INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fft PUBLIC compiler_flags)
