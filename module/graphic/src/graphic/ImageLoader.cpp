#include <fstream>
#include <ranges>
#include "ImageLoader.hpp"
#include "ImageReader.hpp"
#include <core/Buffer.hpp>
#include <core/Platform.hpp>

graphic::ImageLoader::~ImageLoader() {}

std::optional<graphic::Image> graphic::ImageLoader::load(std::string_view filename, std::streambuf *streambuf)
{
    if(readers.empty())
    {
        readers = core::Platform::getInstance().geAllUniqueInstance<graphic::ImageReader>();
    }

    auto minProbe = std::transform_reduce(
        readers.begin(), readers.end(), std::numeric_limits<std::size_t>::min(),
        [](auto a, auto b) { return std::max(a, b); }, [](auto &reader) { return reader->minProbeSize(); });

    auto maxProbe = std::transform_reduce(
        readers.begin(), readers.end(), minProbe, [](auto a, auto b) { return std::max(a, b); },
        [](auto &reader) { return reader->maxProbeSize(); });

    auto available = std::min(streambuf->in_avail(), std::streamsize(4096));
    if(available < 0)
        return std::nullopt;

    core::ByteBuffer<std::byte> buffer;
    buffer.resize(std::max(std::size_t(available), maxProbe));
    ssize_t lastRead;
    do {
        lastRead = buffer.accept(streambuf->sgetn(reinterpret_cast<char *>(buffer.position()), buffer.available()));
    } while(lastRead > 0 && !buffer.full());

    std::vector<ImageReader *> readerSelected;
    readerSelected.reserve(readers.size());
    std::map<graphic::ProbeStatus, std::vector<ImageReader *>> readerRank;

    for(auto &reader : readers)
    {
        auto status_reader = reader->accept(filename, buffer.view());
        readerRank[status_reader].emplace_back(reader.get());
    }

    for(auto &[rank, readerArray] : readerRank | std::views::reverse)
    {
        if(rank == NotRecognized)
            continue;
        for(const auto &reader : readerArray)
        {
            if(auto image = reader->generate(buffer.view(), streambuf))
            {
                return image;
            }
            if(streambuf->pubseekpos(buffer.size(), std::ios::in) < 0)
            {
                return std::nullopt;
            }
        }
    }

    return std::nullopt;
}
