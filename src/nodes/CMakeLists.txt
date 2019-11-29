add_library(nodes
    include/nodes/node.h
    include/nodes/codenode.h
    include/nodes/pagenode.h
    include/nodes/variablenode.h
    include/nodes/viewnode.h
    include/nodes/elementnode.h
    include/nodes/expressionnode.h
    include/nodes/methodnode.h
    include/nodes/statementnode.h
    include/nodes/nodebuildjs.h

    node.cpp
    codenode.cpp
    pagenode.cpp
    variablenode.cpp
    viewnode.cpp
    elementnode.cpp
    expressionnode.cpp
    methodnode.cpp
    statementnode.cpp
    nodebuildjs.cpp)
target_include_directories(nodes PUBLIC include)
target_link_libraries(nodes PUBLIC utils)
