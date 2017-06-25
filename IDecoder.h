//
// Created by ndiladjim on 25/06/17.
// Interface to the Decompression algorithms.
// TODO: Implement DecoderFactory to make usage of decompressor more reusable.
//

#ifndef LZWDECODER_DECODERINTERFACE_H
#define LZWDECODER_DECODERINTERFACE_H

#include <string>
#include <fstream>
class IDecoder {
public:
//    virtual IDecoder& new_instance()=0;
    virtual std::string decompress(const std::string &file_path)=0;
    virtual std::string decompress(std::ifstream &file_stream)=0;
    virtual std::string str()=0;
};
#endif //LZWDECODER_DECODERINTERFACE_H
