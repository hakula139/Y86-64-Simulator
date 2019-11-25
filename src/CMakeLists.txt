# ./src/CMakeLists.txt

aux_source_directory(. DIR_SRCS)
add_subdirectory(assets)
add_subdirectory(init)
add_subdirectory(stages)
add_executable(sim ${DIR_SRCS})
target_link_libraries(sim Assets InitModules Stages)
