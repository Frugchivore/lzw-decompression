//
// Created by ndiladjim on 24/06/17.
//

#include "LZWDecoder.h"

LZWDecoder::LZWDecoder() : IDecoder() {
    dict_head_ = init_dict_size;
    max_size_ = std::pow(2,code_size);
    unsigned int entries_available = max_size_ - init_dict_size;
    dictionary_ = std::vector<std::string>(entries_available);
}

std::string LZWDecoder::decode(const std::string& file_path) {
    decoded_buffer_ = std::stringstream();
    std::vector<Code> code_words;
    read_code_words(file_path, code_words);
    lzw_decode(code_words);
    return get_string();
}

std::string LZWDecoder::decode(std::ifstream& file_stream) {
    decoded_buffer_ = std::stringstream();
    std::vector<Code> code_words;
    char memblock[1];
    file_stream.read(memblock, sizeof(memblock));
    Byte current_byte(memblock[0]);
    Code buffer;
    Code old_code;
    int buffer_pos = code_size;
    int byte_pos = byte_size;
    int cursor = 0;
    while (!file_stream.eof()) {
        byte_pos--;
        bool bit = current_byte.test(byte_pos);
        if (byte_pos <= 0) {
            file_stream.read(memblock, sizeof(memblock));
            current_byte = Byte(memblock[0]);
            byte_pos = byte_size;
        }

        buffer_pos--;
        buffer.set(buffer_pos, bit);
        if (buffer_pos <= 0) {
            lzw_decode_one(cursor, buffer, old_code);
            cursor++;
            buffer.reset();
            buffer_pos = code_size;
        }
    }
    return get_string();
}

std::string LZWDecoder::decode(const std::vector<Code>& code_words) {
    decoded_buffer_ = std::stringstream();
    lzw_decode(code_words);
    return get_string();
}

std::string LZWDecoder::get_string() {
    return decoded_buffer_.str();
}

void LZWDecoder::read_code_words(const std::string &file_path, std::vector<Code> &code_vector) {
    std::ifstream data_file;
    data_file.open(file_path,  std::ios::in|std::ios::binary|std::ios::ate);
    if (data_file.is_open()) {
        std::streampos size = data_file.tellg();
        data_file.seekg(0, std::ios::beg);
        char memblock[1];
        data_file.read(memblock, sizeof(memblock));
        Byte current_byte(memblock[0]);
        Code buffer;
        int buffer_pos = code_size;
        int byte_pos = byte_size;
        int consumed = 0;
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
                code_vector.push_back(buffer);
                buffer.reset();
                buffer_pos = code_size;
            }
        }
        data_file.close();
    } else {
        std::cerr << "error, could not open file" << std::endl;
    }

}

std::string LZWDecoder::translate(Code code_word) {
    ulong index = code_word.to_ulong();
    std::string output;
    if (index < 256) {
        output = static_cast<char>(index);
    } else {
        output = dictionary_.at(index - init_dict_size);
    }
    return output;
}

void LZWDecoder::insert_word(std::string word) {
    if (dict_head_ == max_size_) {
        unsigned int entries_available = max_size_ - init_dict_size;
        dictionary_ = std::vector<std::string>(entries_available);
        dict_head_ = init_dict_size;
    }
    dictionary_.at(dict_head_-init_dict_size) = word;
    dict_head_++;

}

void LZWDecoder::lzw_decode(const std::vector<Code>& code_words) {
    Code old_code = code_words.at(0);
    std::string old_word = translate(old_code);
    char character = old_word.at(0);
    std::string word;
    std::string new_entry;
    decoded_buffer_ << old_word;

    for (std::vector<Code>::const_iterator iter = code_words.begin()+1; iter != code_words.end(); iter++) {

        Code curr_code = *iter;
        if (curr_code.to_ulong() > dict_head_) {
            word = old_word+character;
        } else {
            word = translate(curr_code);
        }
        decoded_buffer_ << word;
        if (word.size() > 0) {
            character = word.at(0);
        } else {
            character = '\0';
        }
        new_entry = old_word+character;
        insert_word(new_entry);
        old_word = word;
    }
}


void LZWDecoder::lzw_decode_one(int cursor, const Code& curr_code, Code& old_code) {
    std::string old_word = translate(old_code);
    char character = old_word.at(0);
    std::string word;
    std::string new_entry;

    if (curr_code.to_ulong() > dict_head_) {
        word = old_word+character;
    } else {
        word = translate(curr_code);
    }
    decoded_buffer_ << word;

    if (word.size() > 0) {
        character = word.at(0);
    } else {
        character = '\0';
    }

    // This will help us manage the first iteration of the algo as we
    // should not start growing the dict by then.
    if (cursor > 0) {
        new_entry = old_word+character;
        insert_word(new_entry);
    }


    old_code = curr_code;
}

void LZWDecoder::reset() {
    decoded_buffer_ = std::stringstream();
    dict_head_ = init_dict_size;
    max_size_ = std::pow(2,code_size);
    unsigned int entries_available = max_size_ - init_dict_size;
    dictionary_ = std::vector<std::string>(entries_available);
}

