add_subdirectory(utils)
add_subdirectory(interface)
add_subdirectory(nodes)
add_subdirectory(compiler)

add_executable(air
    main.cpp)
target_link_libraries(air utils interface)

add_custom_command(TARGET air PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/template/ ${CMAKE_CURRENT_BINARY_DIR})
