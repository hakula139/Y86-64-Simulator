#include <cstdlib>
#include <fstream>
#include <iostream>

#include "config.h"

#include "assets/cpu_clock.h"
#include "assets/file.h"
#include "assets/memory.h"
#include "assets/register.h"
#include "init/argument.h"
#include "stages/bubble.h"
#include "stages/decode.h"
#include "stages/execute.h"
#include "stages/fetch.h"
#include "stages/memory.h"
#include "stages/write_back.h"

using assets::cpu_clock;

int main(int argc, char** argv) {
    initialize::Argument args;
    if (!args.Load(argc, argv)) exit(EXIT_FAILURE);

    assets::File input(args.input_file());
    if (!input.ReadRawData()) exit(EXIT_FAILURE);
    input.ReadInstructions();
    input.Load();
#if SIM_DEBUG
    input.PrintAllInstructions();
#endif

    uint8_t status           = assets::SAOK;
    cpu_clock                = 0ull;
    const uint64_t force_end = 1000ull;
    while (status == assets::SAOK && cpu_clock < force_end) {
        status = stages::WriteBack::Do();
        stages::Memory::Do();
        stages::Execute::Do();
        stages::Decode::Do();
        stages::Fetch::Do(input);
        stages::Bubble::UpdateAll();
        ++cpu_clock;

#if !HAS_GUI
        std::cout << "\nCycle " << std::dec << cpu_clock << ":\n";
        assets::PipelineRegister::Print(assets::FETCH);
        assets::PipelineRegister::Print(assets::DECODE);
        assets::PipelineRegister::Print(assets::EXECUTE);
        assets::PipelineRegister::Print(assets::MEMORY);
        assets::PipelineRegister::Print(assets::WRITE_BACK);
        assets::Memory::Dump();
        assets::Register::Print();
        assets::ConditionCode::Print();
#endif

#if SIM_DEBUG
        std::cout << "Press Enter to continue, Ctrl + C to exit.\n";
        std::getchar();
#endif
    }

#if HAS_GUI
    assets::ChangesHandler::PrintAllInJson();
    assets::ChangesHandler::PrintEnd();
#else
    std::cout << "\nProgram Exit: Clock cycles = " << std::dec << cpu_clock
              << '\n';
#endif
}
