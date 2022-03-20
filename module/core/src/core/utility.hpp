#ifndef BONGOMOUNTAIN_UTILITY_HPP
#define BONGOMOUNTAIN_UTILITY_HPP

#include "config.hpp"
#include <ios>

namespace core {
    BM_CORE_DCL std::string readAll(std::istream &istream);
    BM_CORE_DCL std::string readAll(std::ifstream &istream);
    inline std::string readAll(std::istream &&istream) {
        return readAll(istream);
    }
    inline std::string readAll(std::ifstream &&istream)
    {
        return readAll(istream);
    }
}

#endif// BONGOMOUNTAIN_UTILITY_HPP
