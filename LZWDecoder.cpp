//
// Created by ndiladjim on 24/06/17.
//

#include <cmath>
#include "LZWDecoder.h"

LZWDecoder::LZWDecoder() {
    entries_available_ = std::pow(2,code_size) - init_dict_size;
    dictionary_.reserve(entries_available_);
}

