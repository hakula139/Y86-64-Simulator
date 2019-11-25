#include <cstdlib>
#include <fstream>
#include "init/argument.h"

#include "assets/file.h"

int main(int argc, char** argv) {
    initialize::Argument args;
    if (!args.Load(argc, argv)) exit(EXIT_FAILURE);

    assets::File input(args.input_file());
    if (!input.ReadRawData()) exit(EXIT_FAILURE);
    input.ReadInstructions();
    // input.PrintAllInstructions();
}
