//
// Created by ndiladjim on 24/06/17.
//

#ifndef LZWCODE_LZWDECODER_H
#define LZWCODE_LZWDECODER_H

#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
#include <cmath>

const unsigned int byte_size = 8;
const unsigned int code_size = 12;
const unsigned int init_dict_size = 256;

using Byte = std::bitset<byte_size>;
using Code = std::bitset<code_size>;


class LZWDecoder {
public:
    LZWDecoder();

//    std::string decode(const std::fstream& input_file);
    std::string decode(const std::vector<Code>& code_words);
private:
    std::string translate(Code code_word);
    std::string lzw_decode(const std::vector<Code>& code_words);
    void insert_word(std::string word);

    unsigned int dict_head_;
    unsigned int max_size_;
    std::string decoded_string_;
//    std::stringstream decoded_buffer_;
    std::vector<Code> dictionary_;

};


#endif //LZWCODE_LZWDECODER_H
