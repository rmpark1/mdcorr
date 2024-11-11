file(GLOB sources *.cpp)
add_library(corr ${sources})

target_include_directories(corr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# should we use our own math functions
# option(USE_MYMATH "Use tutorial provided math implementation" ON)

# if (USE_MYMATH)
#   target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")
#   # library that just does sqrt
#   add_library(SqrtLibrary STATIC mysqrt.cxx)
#   # TODO 6: Link SqrtLibrary to tutorial_compiler_flags
#   target_link_libraries(SqrtLibrary PUBLIC tutorial_compiler_flags)
# endif()
target_link_libraries(corr PUBLIC compiler_flags)
