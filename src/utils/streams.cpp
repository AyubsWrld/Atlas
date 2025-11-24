#include "utils/streams.hpp"

[[nodiscard]] inline std::size_t GetStreamSize(const AVFormatContext& ctx, std::size_t streamindex)
{ 
    return ctx.nb_streams;
}

[[nodiscard]] std::size_t GetNumberOfStreams(const AVFormatContext& ctx)
{  
    return ctx.nb_streams;
}
