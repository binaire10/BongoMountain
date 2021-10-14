#include "LightUid.hpp"
LightUid LightUid::generate() {
    LightUid uuid;
    static std::mt19937_64 generator{std::random_device{}()};
    std::uniform_int_distribution<uint8_t> distribution{0, 15};

    for(unsigned i{}; i < sizeof(uuid.id) ; ++i)
        uuid.id = (uuid.id << 8) | distribution(generator) << 4 | distribution(generator);

    return uuid;
}
