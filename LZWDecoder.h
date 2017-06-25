//
// Created by ndiladjim on 24/06/17.
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


class LZWDecoder : public IDecoder {
public:
    LZWDecoder();

//    static IDecoder& new_instance() override;
    std::string decode(const std::string& file_path) override;
    std::string decode(std::ifstream& file_stream);
    std::string decode(const std::vector<Code>& code_words);
    std::string get_string() override;
    void reset();
private:
    std::string translate(Code code_word);
    void read_code_words(const std::string& file_path, std::vector<Code>& code_vector);
    void lzw_decode(const std::vector<Code>& code_words);
    void lzw_decode_one(int cursor, const Code& curr_code, Code& old_code);
    void insert_word(std::string word);
//
    unsigned int dict_head_;
    unsigned int max_size_;
    size_t chunk_size_;
    std::stringstream decoded_buffer_;
    std::vector<std::string> dictionary_;
};

#endif //LZWCODE_LZWDECODER_H
