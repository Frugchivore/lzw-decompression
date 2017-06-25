#include "LZWDecoder.h"
#include <iomanip>
#include <iterator>


int main(int argc, char* argv[]) {
    std::string file_path;
    if (argc > 1) {
        file_path = argv[1];
    } else {
        std::cout << "Please provide path to file to decompress: " << std::endl;
        std::cin >> file_path;
    }

    LZWDecoder decoder;
    std::ifstream data_file;
    data_file.open(file_path,  std::ios::in|std::ios::binary);
    if (data_file.is_open()) {
        std::string output = decoder.decompress(data_file);
        data_file.close();
        std::cout << output << std::endl;
        double codes_count = decoder.codes_count();
        double ratio = 1 - codes_count / output.length();
        std::cout << std::endl << "Compression Ratio: " <<ratio << std::endl;
    } else {
        std::cerr << "[ERROR] could not open file " << file_path;
        return -1;
    }


    return 0;
}




