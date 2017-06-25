//
// Created by ndiladjim on 24/06/17.
// Implementation of the Lempel–Ziv–Welch lossless decompression algorithm.
// operates on a 12 byte code, with base dictionary defined for the first 256 ascii character.
//

#ifndef LZWCODE_LZWDECODER_H
#define LZWCODE_LZWDECODER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
#include <cmath>
#include "IDecoder.h"


const unsigned int byte_size = 8;
const unsigned int code_size = 12;
const unsigned int init_dict_size = 256;

using Byte = std::bitset<byte_size>;
using Code = std::bitset<code_size>;

// Implement IDecoder interface.
// usage:
//    LZWDecoder decoder;
//    std::ifstream data_file;
//    data_file.open(file_path,  std::ios::in|std::ios::binary);
//    std::string output = decoder.decompress(data_file);
//    data_file.close();
class LZWDecoder : public IDecoder {
public:
    LZWDecoder();

    std::string decompress(const std::string &file_path) override;
    std::string decompress(std::ifstream &file_stream) override;


    std::string str() override;

    int codes_count();
    void reset();

private:

    void read_code_words(const std::string& file_path, std::vector<Code>& code_vector);

    void insert_word(std::string word);
    std::string translate(Code code_word);

    void lzw_decode(const std::vector<Code>& code_words);
    void lzw_decode_one(int cursor, const Code& curr_code, Code& old_code);

    std::string decompress(const std::vector<Code> &code_words);

    unsigned int num_code_words_;
    unsigned int dict_head_;
    int max_size_;
    std::stringstream decoded_buffer_;
    std::vector<std::string> dictionary_;
};

#endif //LZWCODE_LZWDECODER_H
