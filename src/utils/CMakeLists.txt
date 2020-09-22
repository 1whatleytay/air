add_library(utils
    include/utils/parser.h
    include/utils/file.h
    include/utils/charprops.h

    parser.cpp
    file.cpp
    charprops.cpp)
target_include_directories(utils PUBLIC include)
target_link_libraries(utils PUBLIC fmt)
