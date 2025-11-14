#include "pcm.hpp"


namespace ATLAS
{

    [[nodiscard]] 
    int PrepareDecoderForStream(const AVCodec** codec, AVFormatContext* format_context, AVCodecContext* decoder_ctx, AVMediaType mediatype)
    {
        AVStream*    best_fit_stream{};
        int          best_stream_index{-1}; 
        int          return_val{-1}; 

        if(decoder_ctx == NULL)
        {
            //spdlog::error("[Error]: Prior to using this function the context has to be allocated with avcodec_alloc_context3().");
            return return_val; 
        }

        return_val          =   av_find_best_stream(
                                                    format_context,
                                                    mediatype,
                                                    -1,
                                                    -1,
                                                    codec,
                                                    0
                                                );
        /* In the case that a "bestfit" stream was not found return early */ 

        if(return_val < 0)
        {
            //spdlog::error("[Error]: Failed to find best fit stream.");
            return return_val ;
        }

        /* In the case it was return return_value should be a non-negative index for the stream we wish to process */ 

        best_stream_index = return_val;
        best_fit_stream = format_context->streams[best_stream_index];

        /* Provide Additional context to decoder */ 

        if(return_val = avcodec_parameters_to_context(decoder_ctx, best_fit_stream->codecpar) ; return_val < 0)
        {
            //spdlog::error("[Error]:Failed while attempting to copy best fit stream parameters to decoder context.");
            return return_val;
        }

        if(return_val = avcodec_open2(decoder_ctx, *codec, NULL) ; return_val != 0)
        {
            //spdlog::error("[Error]:Failed to initialize the AVCodecContext to use the given AVCodec.");
            return return_val; 
        }
        return best_stream_index; 
    }

    int ReadStreamPMC() { std::cout << "Hello Wolrd" << std::endl; ; return -1; }
}
