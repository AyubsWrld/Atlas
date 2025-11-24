#pragma once

extern "C"
{
#include <libavformat/avformat.h>
}

namespace ATLAS
{
    [[nodiscard]] std::size_t GetStreamSize(const AVFormatContext& ctx);
    [[nodiscard]] std::size_t GetNumberOfStreams(const AVFormatContext& ctx);
}
