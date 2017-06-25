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

// Read content of file in memory before parsing the resulting code sentence.
std::string LZWDecoder::decompress(const std::string &file_path) {


    decoded_buffer_ = std::stringstream();
    std::vector<Code> code_words;
    read_code_words(file_path, code_words);
    return decompress(code_words);
}

// Consume file stream, parse code words and convert them on the fly.
// Does not have the overhead of reading the whole file in memory and touch each byte once.
// For larger file prefer this method to LZWDecoder::decompress(const std::string &file_path).
// Closing the file stream is the responsibility of the client.
std::string LZWDecoder::decompress(std::ifstream &file_stream) {


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
    num_code_words_ = cursor;
    return str();
}

// Helper function for LZWDecoder::decompress(const std::string &file_path).
// Operates on an existing code_words, preferably created by
// LZWDecoder::read_code_words(const std::string &file_path, std::vector<Code> &code_vector).
std::string LZWDecoder::decompress(const std::vector<Code> &code_words) {

    decoded_buffer_ = std::stringstream();
    lzw_decode(code_words);
    return str();
}

// Accessor for the decompressed string.
std::string LZWDecoder::str() {
    return decoded_buffer_.str();
}

// Read the content of a file into a code words vector for use by a decompression routine.
// The EOF is probed to get the size of the file. Then bytes are processed until a code can
// be created. The code is added to the code_vector and the process repeats until all bytes have been
// consumed.
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
            //notice that the cursor only moves if all the bits of the current byte have been consumed.
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
        std::cerr << "[ERROR] could not open file " << file_path << std::endl;
    }

}

// Translate a bitset code word into the appropriate character.
// The inner dictionary only operates on the new codes. For any code in the
// base dictionary, we just convert to char.
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

// make a new code word available in the dictionary. If the cursor of the dictionary reaches the
// limit, the cursor is reset and the dictionary is flushed.
void LZWDecoder::insert_word(std::string word) {
    if (dict_head_ == max_size_) {
        unsigned int entries_available = max_size_ - init_dict_size;
        dictionary_ = std::vector<std::string>(entries_available);
        dict_head_ = init_dict_size;
    }
    dictionary_.at(dict_head_-init_dict_size) = word;
    dict_head_++;

}

// Core implementation of the LZW decompression algorithm.
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

// Decode one code words. Used in the streaming implementation of the decompression routine.
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

// Reset the decoder.
void LZWDecoder::reset() {
    num_code_words_ = 0;
    decoded_buffer_ = std::stringstream();
    dict_head_ = init_dict_size;
    max_size_ = std::pow(2,code_size);
    unsigned int entries_available = max_size_ - init_dict_size;
    dictionary_ = std::vector<std::string>(entries_available);
}

// get count of codes read during last run of the decompression.
int LZWDecoder::codes_count() {
    return num_code_words_;
}

