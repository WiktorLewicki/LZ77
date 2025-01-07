#include "LZ77_Decompressor.h"
int LZ77_DECOMPRESSOR::readInt(std::ifstream &file) {
    unsigned int x = 0;
    char byte;
    for (int i = 0; i < 3; i++) {
        file.get(byte);
        x |= (static_cast<unsigned char>(byte) << (i * 8));
    }
    return x;
}
int LZ77_DECOMPRESSOR::decompress(const std::string &FileIN, const std::string &FileOUT) {
    std::ifstream IN(FileIN, std::ios::binary);
    if (!IN.is_open()) {
        return 1;
    }
    std::ofstream OUT(FileOUT, std::ios::binary);
    if (!OUT.is_open()) {
        return 2;
    }
    int FILE_START = readInt(IN);
    if(FILE_START != 7999999u){
        return 3;
    }
    int BLOCK_END = readInt(IN);
    int FILE_END = readInt(IN);
    std::string s = "";
    int l, r;
    char c;
    while ((l = readInt(IN)) != FILE_END) {
        if (l == BLOCK_END) {
            if(s.back() == '~') s.pop_back();
            for(auto &v : s) OUT.put(v);
            s = "";
            continue;
        }
        r = readInt(IN);
        IN.get(c);
        if (l != 0) {
            int p = (int)s.size();
            p -= l;
            for (int i = p; i < p + r; i++) {
                s += s[i];
            }
        }
        s += c;
    }
    IN.close();
    OUT.close();
    return 0;
}
