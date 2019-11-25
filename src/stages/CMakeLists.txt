# ./src/stages/CMakeLists.txt

aux_source_directory(. DIR_LIB_SRCS)
add_library(Stages STATIC
            ${DIR_LIB_SRCS})
