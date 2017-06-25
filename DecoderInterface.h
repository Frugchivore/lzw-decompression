//
// Created by ndiladjim on 25/06/17.
//

#ifndef LZWDECODER_DECODERINTERFACE_H
#define LZWDECODER_DECODERINTERFACE_H

#include <string>
#include <fstream>
class DecoderInterface {
public:

    virtual std::string decode(const std::string& file_path)=0;
    virtual std::string get_string()=0;
};
#endif //LZWDECODER_DECODERINTERFACE_H
