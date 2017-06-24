//
// Created by ndiladjim on 24/06/17.
//

#include "LZWDecoder.h"

LZWDecoder::LZWDecoder() {
    dict_head_ = init_dict_size;
    max_size_ = std::pow(2,code_size);
    entries_available = std::pow(2,code_size) - init_dict_size;
    dictionary_.reserve(entries_available);
}

std::string LZWDecoder::decode(const std::vector<Code>& code_words) {
    return lzw_decode(code_words);
}

std::string LZWDecoder::translate(Code code_word) {
    ulong index = code_word.to_ulong();
    std::string output;
    if (index < 256) {
        output = static_cast<char>(index);
    } else {
        output = dictionary_.at(index);
    }
    return output;
}

void LZWDecoder::insert_word(std::string word) {
    if (dict_head_ == max_size_) {
        dictionary_.clear();
        dict_head_ = init_dict_size;
    }
    dictionary_.at(dict_head_) = word;
    dict_head_++;

}

std::string LZWDecoder::lzw_decode(const std::vector<Code>& code_words) {
    std::stringstream output;
    int count_read = 0;
    Code old_code = code_words.at(count_read);
    std::string old_word = translate(old_code);
    char character = old_word.at(0);
    std::string word;
    output << word;
    while (count_read < code_words.size()) {
        count_read++;
        Code curr_code = code_words.at(count_read);
        if (curr_code.to_ulong() > dict_head_) {
            word = old_word+character;
        } else {
            word = translate(curr_code);

        }
        output << word;
        character = word.at(0);
        insert_word(old_word+character);
        old_word = word;
    }
    decoded_string_ = output.str();
    return decoded_string_;
}
