#include "LZWDecoder.h"
#include <iostream>
#include <iomanip>
#include <iterator>



//typedef std::bitset<byte_size> Byte;
//typedef std::bitset<code_size> Code;

void read_binary_file(const std::string& file_path, std::vector<Byte>& binary_data);
void read_binary_file_to_code(const std::string& file_path, std::vector<Code>& binary_data);

template <typename Type>
void print_vector(std::vector<Type> array);

int main(int argc, char* argv[]) {
    std::string file_path;
    if (argc > 1) {
        file_path = argv[1];
    } else {
        file_path = "/home/ndiladjim/source_repository/LZWDecoder/LzwInputData/compressedfile1.z";
    }

    std::vector<Code> codewords;
    read_binary_file_to_code(file_path, codewords);

    print_vector<Code>(codewords);
    LZWDecoder decoder;
//    std::string output = decoder.decode(codewords);
//    std::cout << output << std::endl;
    return 0;
}


void read_binary_file(const std::string& file_path, std::vector<Byte>& binary_data) {
    std::ifstream data_file;

    data_file.open(file_path,  std::ios::in|std::ios::binary|std::ios::ate);
    if (data_file.is_open()) {
        data_file.unsetf(std::ios::skipws);
        std::cout << "file opened successfully" << std::endl;
        std::streampos size = data_file.tellg();
        data_file.seekg (0, std::ios::beg);
        std::cout << "size: " << size << std::endl;
        char memblock[1];
        for (int i = 0; i < size; i++) {
            data_file.read(memblock, 1);
            binary_data.push_back(Byte(memblock[0]));
        }

        data_file.close();

        std::cout << std::endl << "file closed successfully" << std::endl;
    } else {
        std::cerr << "error, could not open file" << std::endl;
    }

}

void read_binary_file_to_code(const std::string& file_path, std::vector<Code>& code_vector) {
    std::ifstream data_file;

    data_file.open(file_path,  std::ios::in|std::ios::binary|std::ios::ate);
    if (data_file.is_open()) {
        data_file.unsetf(std::ios::skipws);
        std::cout << "file opened successfully" << std::endl;
        std::streampos size = data_file.tellg();
        data_file.seekg (0, std::ios::beg);
        std::cout << "size: " << size << std::endl;
        char memblock[1];
        int consumed = 0;
        data_file.read(memblock, sizeof(memblock));
        Byte current_byte(memblock[0]);
        Code buffer;
        int buffer_pos = code_size;
        int byte_pos = byte_size;
        while (consumed < size) {
            byte_pos--;
            bool bit = current_byte.test(byte_pos);
            if (byte_pos <= 0) {
                data_file.read(memblock, sizeof(memblock));
                current_byte = Byte(memblock[0]);
                consumed++;
                byte_pos = byte_size;
            }
            buffer_pos--;
            buffer.set(buffer_pos, bit);

            if (buffer_pos <= 0) {

//                ulong index = buffer.to_ulong();
//                std::string output;
//                output = static_cast<char>(index);
//                std::cout << buffer<<" " << index << " " << output << " | ";
                code_vector.push_back(buffer);
                buffer.reset();
                buffer_pos = code_size;
            }
        }
        data_file.close();

        std::cout << std::endl << "file closed successfully" << std::endl;
    } else {
        std::cerr << "error, could not open file" << std::endl;
    }

}

template <typename Type>
void print_vector(std::vector<Type> array) {
    for (typename std::vector<Type>::const_iterator iter = array.begin(); iter != array.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}

std::string translate(Code code) {
    ulong index = code.to_ulong();
    std::string output;
    if (index < 256) {
        output = static_cast<char>(index);
    }
}
//
//std::string lzw(std::vector<Code> code_array) {
//    std::stringstream output;
//    int dict_head = 256;
//    int count_read = 0;
//    Code old_code = code_array.at(count_read);
//    std::string old_word = translate(old_code);
//    char character = old_word.at(0);
//    std::string word;
//    output << word;
//    while (count_read < code_array.size()) {
//        count_read++;
//        Code curr_code = code_array.at(count_read);
//        if (curr_code.to_ulong() > dict_head) {
//            word = old_word+character;
//        } else {
//            word = translate(curr_code);
//
//        }
//        output << word;
//        character = word.at(0);
//        insert_word(dict_head, old_word+character);
//        old_word = word;
//    }
//
//    return output.str();
//}

//Read OLD_Code
//output OLD_Code
//CHARACTER = OLD_Code
//WHILE there are still input characters DO
//        Read NEW_Code
//        IF NEW_Code is not in the translation table THEN
//          STRING = get translation of OLD_Code
//          STRING = STRING+CHARACTER
//        ELSE
//          STRING = get translation of NEW_Code
//        END of IF
//        output STRING
//        CHARACTER = first character in STRING
//        add OLD_Code + CHARACTER to the translation table
//        OLD_Code = NEW_Code
//END of WHILE