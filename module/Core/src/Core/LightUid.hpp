#ifndef GAMEENGINEBONGO_LIGHTUID_HPP
#define GAMEENGINEBONGO_LIGHTUID_HPP

#include <array>
#include <cstdint>
#include <random>

#include "config.hpp"

class BM_CORE_DCL LightUid
{
public:
    static LightUid generate();

    inline uint32_t getId() const noexcept { return id; }

    inline bool operator==(const LightUid &rhs) const noexcept { return id == rhs.id; }

    inline bool operator!=(const LightUid &rhs) const noexcept { return !(rhs == *this); }

private:
    uint32_t id;
};


#endif// GAMEENGINEBONGO_LIGHTUID_HPP
