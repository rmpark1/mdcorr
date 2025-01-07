cmake_minimum_required(VERSION 3.13)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# set the project name and version
project(mdcorr VERSION 1.0)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)
if (PARALLEL)
    add_compile_definitions(-DPARALLEL PARALLEL)
endif()
if (OPT)
    add_compile_options(-DOfast)
endif()
if (DEV)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()


configure_file(mdcorr.h.in mdcorr.h)

add_subdirectory(array)
add_subdirectory(corr)
add_subdirectory(fft)
add_subdirectory(parse)

# add the executable
add_executable(mdcorr mdcorr.cpp)

target_link_libraries(parse PUBLIC array compiler_flags)
# target_link_libraries(fft PUBLIC array compiler_flags)
target_link_libraries(corr PUBLIC array fft compiler_flags)

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
        test_valid_file
        test_parallel
        test_fft
        test_read
    )

    foreach(TFI ${TEST_FILES})
        add_executable(${TFI} tests/${TFI}.cpp)
        target_link_libraries(${TFI} corr_sh)
        add_test(${TFI} ${TFI})
    endforeach()
endif()
