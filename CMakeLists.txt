cmake_minimum_required(VERSION 3.13)

# set the project name and version
project(mdcorr VERSION 1.0)

# * Creating an interface library called tutorial_compiler_flags
#   Hint: use add_library() with the INTERFACE signature
add_library(compiler_flags INTERFACE)
# * Add compiler feature cxx_std_11 to tutorial_compiler_flags
target_compile_features(compiler_flags INTERFACE cxx_std_20)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(mdcorr.h.in mdcorr.h)

add_subdirectory(array)
add_subdirectory(fft)
add_subdirectory(parse)

# add the executable
add_executable(mdcorr mdcorr.cpp)

target_link_libraries(parse PUBLIC array compiler_flags)

target_link_libraries(mdcorr PUBLIC fft parse compiler_flags)
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(mdcorr PUBLIC "${PROJECT_BINARY_DIR}")

add_library(corr SHARED mdcorr.cpp)
target_link_libraries(corr PUBLIC fft parse)
