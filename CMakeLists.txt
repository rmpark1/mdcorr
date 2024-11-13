cmake_minimum_required(VERSION 3.13)

# set the project name and version
project(mdcorr VERSION 1.0)

# * Creating an interface library called tutorial_compiler_flags
#   Hint: use add_library() with the INTERFACE signature
add_library(compiler_flags INTERFACE)
# * Add compiler feature cxx_std_11 to tutorial_compiler_flags
target_compile_features(compiler_flags INTERFACE cxx_std_20)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(mdcorr.h.in mdcorr.h)

add_subdirectory(array)
add_subdirectory(corr)
add_subdirectory(parse)

# add the executable
add_executable(mdcorr mdcorr.cpp)

target_link_libraries(parse PUBLIC array compiler_flags)
target_link_libraries(corr PUBLIC array compiler_flags)

target_link_libraries(mdcorr PUBLIC corr parse compiler_flags)
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(mdcorr PUBLIC "${PROJECT_BINARY_DIR}")

add_library(corr_sh SHARED mdcorr.cpp)
target_link_libraries(corr_sh PUBLIC corr parse compiler_flags)

# TESTING
if (BUILD_TESTING)

    enable_testing()

    set(TEST_FILES
        test_array_iter
        test_array_mem
        test_array
        test_cli
        test_utils
        test_correlate
    )

    foreach(TFI ${TEST_FILES})
        add_executable(${TFI} tests/${TFI}.cpp)
        target_link_libraries(${TFI} corr_sh)
        add_test(${TFI} ${TFI})
    endforeach()
endif()
