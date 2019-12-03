#ifndef SRC_ASSETS_FILE_H_
#define SRC_ASSETS_FILE_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace assets {

// Reads the file; extracts and stores all instructions
class File {
public:
    explicit File(const std::string& input_file) : input_file_(input_file) {}

    bool                 ReadRawData();
    bool                 ReadInstructions();
    std::vector<uint8_t> GetInstruction(uint64_t address,
                                        bool*    mem_error) const;
    bool                 ReachEOF(uint64_t address) const;
    // Loads all instructions into memory
    bool Load() const;

    bool PrintRawData() const;
    bool PrintInstruction(uint64_t address) const;
    bool PrintAllInstructions() const;

protected:
    bool PrintErrorMessage(const int error_code) const;

    std::string              input_file_;
    std::vector<std::string> raw_data_;
    uint64_t                 max_address_ = 0ull;
    // stores the address and the instruction code
    std::map<uint64_t, std::vector<uint8_t>> instructions_;
};

}  // namespace assets

#endif  // SRC_ASSETS_FILE_H_
