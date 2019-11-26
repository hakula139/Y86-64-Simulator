#include <cstdlib>
#include <fstream>

#include "config.h"

#include "init/argument.h"

#include "assets/file.h"

#include "stages/fetch.h"

int main(int argc, char** argv) {
    initialize::Argument args;
    if (!args.Load(argc, argv)) exit(EXIT_FAILURE);

    assets::File input(args.input_file());
    if (!input.ReadRawData()) exit(EXIT_FAILURE);
    input.ReadInstructions();
#if SIM_DEBUG
    input.PrintAllInstructions();
#endif

    bool end = false;
    while (end) { stages::Fetch::Do(input); }
}
