#ifndef LZ77_COMPRESSOR_H
#define LZ77_COMPRESSOR_H

#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>
#include "KMR.h"

#define FILE_TERMINATOR 1000075u
#define BLOCK_TERMINATOR 1000099u
#define START_OF_FILE 7999999u
#define BLOCK_SIZE (1 << 16)

class LZ77_COMPRESSOR {
private:
    void writeInt(const unsigned int x, std::ofstream &pf);
    std::vector<std::array<int, 3>> compress_block(const std::string &s);

public:
    LZ77_COMPRESSOR();
    int compress(const std::string FileIN, const std::string FileOUT);
};

#endif // LZ77_COMPRESSOR_H
