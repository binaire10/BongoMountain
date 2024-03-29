#ifndef BONGOMOUNTAIN_LIBRARY_HPP
#define BONGOMOUNTAIN_LIBRARY_HPP

#include <string_view>
#include <optional>

#ifdef WIN32
#include <windows.h>
#    include "memory.hpp"

class Library
{
public:
    inline Library(std::string_view file)
    {
        hhandle = fixe_string(file, [](auto str) { return LoadLibraryA(str.data()); });
    }

    Library(const Library &) = delete;
    Library(Library &&move) noexcept : hhandle{*move.hhandle} {
        move.hhandle = std::nullopt;
    }

    template<typename FuncT>
    FuncT *load(std::string_view symbol)
    {
        return reinterpret_cast<FuncT*>(fixe_string(symbol, [this](auto str) { return GetProcAddress(*hhandle, str.data()); }));
    }

    ~Library() { if(hhandle) FreeLibrary(*hhandle); }

private:
    std::optional<HMODULE> hhandle;
};
#endif

#endif// BONGOMOUNTAIN_LIBRARY_HPP
