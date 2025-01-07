#ifndef LZ77_DECOMPRESSOR_H
#define LZ77_DECOMPRESSOR_H

#include <iostream>
#include <fstream>
#include <string>

class LZ77_DECOMPRESSOR {
private:
    int readInt(std::ifstream &file);
public:
    int decompress(const std::string &FileIN, const std::string &FileOUT);
};

#endif // LZ77_DECOMPRESSOR_H
