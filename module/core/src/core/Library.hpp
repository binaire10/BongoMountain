#ifndef BONGOMOUNTAIN_LIBRARY_HPP
#define BONGOMOUNTAIN_LIBRARY_HPP

#include <string_view>
#include <filesystem>
#include <optional>


#ifdef WIN32
#    include <windows.h>
#    include "memory.hpp"
#endif

namespace core
{
    inline namespace v1_0
    {
#ifdef WIN32
        class Library
        {
        public:
            Library() = default;
            inline Library(const std::filesystem::path &file) { hhandle = LoadLibraryW(file.c_str()); }

            inline Library(std::string_view file)
            {
                hhandle = fixe_string([](auto str) { return LoadLibraryA(str.data()); }, file);
            }

            Library &operator=(const Library &) = delete;
            Library(const Library &)            = delete;
            inline Library(Library &&move) noexcept
                : hhandle{ std::move(move.hhandle) }
            {
                move.hhandle = std::nullopt;
            }

            inline void open(std::string_view file)
            {
                if(hhandle)
                    FreeLibrary(*hhandle);
                hhandle = fixe_string([](auto str) { return LoadLibraryA(str.data()); }, file);
            }

            inline void close()
            {
                if(hhandle)
                    FreeLibrary(*hhandle);
            }

            template<typename FuncT>
            FuncT *load(std::string_view symbol)
            {
                return reinterpret_cast<FuncT *>(
                    fixe_string([this](auto str) { return GetProcAddress(*hhandle, str.data()); }, symbol));
            }

            inline ~Library() { Library::close(); }

            HMODULE get_handler() const { return *hhandle; }

        private:
            std::optional<HMODULE> hhandle;
        };
#endif
    }// namespace v1_0
}// namespace core
#endif// BONGOMOUNTAIN_LIBRARY_HPP
