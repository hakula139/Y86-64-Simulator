# ./src/init/CMakeLists.txt

aux_source_directory(. DIR_LIB_SRCS)
add_library(InitModules STATIC
            ${DIR_LIB_SRCS})
