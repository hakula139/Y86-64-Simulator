# ./src/CMakeLists.txt

aux_source_directory(. DIR_SRCS)
add_subdirectory(init)
add_executable(sim ${DIR_SRCS})
target_link_libraries(sim InitModules)
