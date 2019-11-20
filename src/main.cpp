#include <cstdlib>
#include <fstream>
#include <iostream>

#include "init/argument.h"

int main(int argc, char** argv) {
    initialize::Argument args;
    if (!args.Load(argc, argv)) exit(EXIT_FAILURE);
    std::string input_file(args.input_file());
    std::cout << "Successfully read " << input_file << '\n';
    std::ifstream input;
    input.open(input_file, std::ifstream::in);
    input.close();
}
