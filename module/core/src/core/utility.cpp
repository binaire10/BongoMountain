#include "utility.hpp"
#include <fstream>

std::string core::readAll(std::istream &istream) {
        istream.exceptions(std::ios::failbit);
        std::string buffer(std::istreambuf_iterator<char>{ istream }, std::istreambuf_iterator<char>{});
        return buffer;
}

std::string core::readAll(std::ifstream &istream) {
    istream.exceptions(std::ios::failbit);
    istream.seekg(0, std::ios::end);
    auto        count = istream.tellg();
    std::string buffer(count, '\0');
    istream.seekg(0, std::ios::beg);
    istream.read(buffer.data(), count);
    return buffer;
}
