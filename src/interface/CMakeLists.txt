add_library(interface
    include/interface/interface.h

    interface.cpp)
target_include_directories(interface PUBLIC include)
target_link_libraries(interface PRIVATE compiler utils)
