//
// Created by ndiladjim on 25/06/17.
//

#ifndef LZWDECODER_DECODERFACTORY_H
#define LZWDECODER_DECODERFACTORY_H

#include <iostream>
#include <map>
#include <string>
#include <functional>
#include "IDecoder.h"

class DecoderFactory {
public:
    typedef std::function<DecoderInterface> func;
    static DecoderFactory& instance() {
        static DecoderFactory factory;
        return factory;
    }

    static void register_decoder(std::string algorithm_name,  func factory_func) {
        DecoderFactory factory = instance();
        factory.decoders.insert(std::pair<std::string, func>(algorithm_name, factory_func));
    }

    DecoderInterface& get_decoder(std::string algorithm_name) {
        std::map<std::string, std::function<DecoderInterface>>::const_iterator idx = decoders.find(algorithm_name);

        if (idx = decoders.end()) {
            std::cerr << "[ERROR]: Selected decompression algorithm is not available.";
            return nullptr;
        }

        return idx->second;
    }
private:
    DecoderFactory();

    std::map<std::string, std::function<DecoderInterface>> decoders;
};


#endif //LZWDECODER_DECODERFACTORY_H
