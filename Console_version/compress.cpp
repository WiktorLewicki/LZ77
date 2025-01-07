#include <iostream>
#include "modules/LZ77_Compressor.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1; // Zwraca błąd, jeśli nie podano wymaganych argumentów
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    LZ77_COMPRESSOR compressor;
    int result = compressor.compress(inputFile, outputFile);

    if (result == 0) {
        std::cout << "File successfully compressed." << std::endl;
    } else if (result == 1) {
        std::cerr << "Error: Could not open input file." << std::endl;
    } else if (result == 2) {
        std::cerr << "Error: Could not open output file." << std::endl;
    } else {
        std::cerr << "Error: An unknown error occurred during compression." << std::endl;
    }

    return result;
}
