#include <cstdlib>
#include <fstream>
#include <iostream>

#include "config.h"

#include "assets/file.h"
#include "assets/register.h"
#include "init/argument.h"
#include "stages/decode.h"
#include "stages/execute.h"
#include "stages/fetch.h"
#include "stages/memory.h"
#include "stages/write_back.h"

int main(int argc, char** argv) {
    initialize::Argument args;
    if (!args.Load(argc, argv)) exit(EXIT_FAILURE);

    assets::File input(args.input_file());
    if (!input.ReadRawData()) exit(EXIT_FAILURE);
    input.ReadInstructions();
#if SIM_DEBUG
    input.PrintAllInstructions();
#endif

    uint8_t  status = assets::SAOK;
    uint64_t clock  = 0ull;
    while (status == assets::SAOK) {
        status = stages::WriteBack::Do();
        stages::Memory::Do();
        stages::Execute::Do();
        stages::Decode::Do();
        stages::Fetch::Do(input);
        ++clock;

        assets::PipelineRegister::Print(assets::FETCH);
        assets::PipelineRegister::Print(assets::DECODE);
        assets::PipelineRegister::Print(assets::EXECUTE);
        assets::PipelineRegister::Print(assets::MEMORY);
        assets::PipelineRegister::Print(assets::WRITE_BACK);
        assets::Register::Print();
        assets::ConditionCode::Print();
        std::cout << "Press enter to continue, CTRL + C to exit.\n";
        std::getchar();
    }
    std::cout << "Program Exit: Clock cycles = " << clock << '\n';
}
