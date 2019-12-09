# ./src/assets/CMakeLists.txt

aux_source_directory(. DIR_LIB_SRCS)
add_library(Assets STATIC
            ${DIR_LIB_SRCS})
