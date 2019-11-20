#include <cstdlib>
#include <iostream>

#include "init/argument.h"

int main(int argc, char** argv) {
    initialize::Argument args;
    if (!args.Load(argc, argv)) exit(EXIT_FAILURE);
    std::cout << "Successfully read " << args.input_file() << '\n';
}
