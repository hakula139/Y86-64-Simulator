# ./src/utils/CMakeLists.txt

aux_source_directory(. DIR_LIB_SRCS)
add_library(Utilities STATIC
            ${DIR_LIB_SRCS})
