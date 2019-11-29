add_library(compiler
    include/compiler/compiler.h

    compiler.cpp)
target_include_directories(compiler PUBLIC include)
target_link_libraries(compiler PRIVATE nodes)
