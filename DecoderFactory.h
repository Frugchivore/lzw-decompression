//
// Created by ndiladjim on 25/06/17.
//

#ifndef LZWDECODER_DECODERFACTORY_H
#define LZWDECODER_DECODERFACTORY_H

#include <map>
#include <string>

class DecoderFactory {
public:
    static DecoderFactory& instance();
    void register_decoder(std::string key, DecoderBuilder)
private:
    DecoderFactory();
};


#endif //LZWDECODER_DECODERFACTORY_H
