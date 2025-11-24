#include "pcm.hpp"


namespace Atlas
{
    [[nodiscard]] 
    int PrepareDecoderForStream(const AVCodec** codec, AVFormatContext* format_context, AVCodecContext* decoder_ctx, AVMediaType mediatype)
    {
        AVStream*    best_fit_stream{};
        int          best_stream_index{-1}; 
        int          return_val{-1}; 

        if(decoder_ctx == NULL)
        {
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
            return return_val ;
        }

        /* In the case it was return return_value should be a non-negative index for the stream we wish to process */ 

        best_stream_index = return_val;
        best_fit_stream = format_context->streams[best_stream_index];

        /* Provide Additional context to decoder */ 

        /* Throws segfault here */ 
        if(return_val = avcodec_parameters_to_context(decoder_ctx, best_fit_stream->codecpar) ; return_val < 0)
        {
            return return_val;
        }

        if(return_val = avcodec_open2(decoder_ctx, *codec, NULL) ; return_val != 0)
        {
            return return_val; 
        }
        return best_stream_index; 
    }

    void SplitAudioStream() 
    // void SplitAudioStream(const std::string& filename, char* buffer) 
    {
        const AVCodec*      Decoder; 
        AVFormatContext*    FormatContext  {avformat_alloc_context()}; 
        AVCodecContext*     DecoderContext {avcodec_alloc_context3(Decoder)};
        // AVCodecContext*     DecoderContext {avcodec_alloc_context3(Decoder)};

        int ReturnValue{};

        // if(DecoderContext == NULL)
        // {
        //     std::cout << "Could Not set DecoderContext";
        //     return;
        // }
        
        ReturnValue = avformat_open_input(
                                          &FormatContext,
                                          "/mnt/e/atlas/examples/ffmpeg/codecs/input.mp4",
                                          NULL,
                                          NULL
                                          );
        if(ReturnValue != 0)
        {
            std::cout << "[" << __FUNCTION__ << "]: Failed call to `avformat_open_input`, got: " << ReturnValue << std::endl;
        }

        ReturnValue = PrepareDecoderForStream(
                                              &Decoder,
                                              FormatContext,
                                              DecoderContext,
                                              AVMEDIA_TYPE_AUDIO
                                              );

        if(ReturnValue > 0)
        {
            std::cout << "It worked" << std::endl;
        }
    }
}
