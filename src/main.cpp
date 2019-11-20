#include <cstdlib>
#include <fstream>
#include <iostream>

#include "init/argument.h"

int main(int argc, char** argv) {
    initialize::Argument args;
    if (!args.Load(argc, argv)) exit(EXIT_FAILURE);

    std::ifstream input;
    input.open(args.input_file(), std::ifstream::in);
    input.close();
}
