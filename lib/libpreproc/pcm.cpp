#include "pcm.hpp"


namespace Atlas
{

    [[nodiscard]] 
    int PrepareDecoderForStream(const AVCodec** codec, AVFormatContext* format_context, AVCodecContext** decoder_ctx, AVMediaType mediatype)
    {
        AVStream*    best_fit_stream{};
        int          best_stream_index{-1}; 
        int          return_val{-1}; 

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


        /* #TODO: Removing this line causes a memory access violation explain why.*/ 
        *decoder_ctx = avcodec_alloc_context3(*codec);

        if(*decoder_ctx == NULL)
        {
            std::cout << "Failed call to `avcodec_alloc_context3`" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        /* Throws segfault here */ 
        if(return_val = avcodec_parameters_to_context(*decoder_ctx, best_fit_stream->codecpar) ; return_val < 0)
        {
            return return_val;
        }

        if(return_val = avcodec_open2(*decoder_ctx, *codec, NULL) ; return_val != 0)
        {
            return return_val; 
        }
        return best_stream_index; 
    }

    void SplitAudioStream() 
    // void SplitAudioStream(const std::string& filename, char* buffer) 
    {
        const AVCodec*      Decoder; 
        AVFormatContext*    FormatContext  { NULL };  // Allocated during the call ot `avformat_open_input` so a call to allocate is superfluous.
        AVCodecContext*     DecoderContext { NULL };
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

        if(avformat_find_stream_info(FormatContext, NULL) < 0)
        {
            std::cout << "Could not find stream info" << std::endl; 
            std::exit(EXIT_FAILURE);
        }

        if(ReturnValue != 0)
        {
            std::cout << "[" << __FUNCTION__ << "]: Failed call to `avformat_open_input`, got: " << ReturnValue << std::endl;
        }

        /* This is the prepares best fit stream & and deocder for handling the best fit stream */ 
        ReturnValue = PrepareDecoderForStream(
                                              &Decoder,
                                              FormatContext,
                                              &DecoderContext,
                                              AVMEDIA_TYPE_AUDIO
                                              );

        if(ReturnValue >= VALID_STREAM_INDEX)
        {
            std::cout << "It worked" << std::endl;
            ReadAudioStream(FormatContext, DecoderContext, ReturnValue);
        }

    }

    constexpr std::size_t GetAudioFileSize(const AVFormatContext& fmt) noexcept
    {
        /* Check whether there is enough information to derive this */ 
        return ((fmt.bit_rate / 1024 ) * fmt.duration) ;
    }

    __attribute__((pure)) constexpr std::size_t GetAudioFileSize(const AVFormatContext* fmt) noexcept
    {
        /* Check whether there is enough information to derive this */ 
        return ((fmt->bit_rate / 1024 ) * fmt->duration) ;
    }

    /* Perhaps inline all of this into a single routine */ 
    void ReadAudioStream(AVFormatContext* format_ctx, AVCodecContext* decoder_ctx, int best_stream_index)
    {
        /* Assertions that must hold */ 
        /* These could be runtime failures exit prematurely */ 
        // assert(best_stream_index < VALID_STREAM_INDEX && std::format("Ill-formed (negative) stream_index received: {}\n", best_stream_index));
        // assert(best_stream_index < format_ctx->nb_streams && std::format("Ill-formed stream_index received: {}\n, greater than available streams within format", best_stream_index));

        AVPacket* Packet { av_packet_alloc() };
        AVFrame*  Frame  { av_frame_alloc() };
        AVStream* BestStream = format_ctx->streams[best_stream_index];

        // assert(  (!Packet || !Frame) && std::format("Failed to allocate Frame({})/Packet({})", Frame, Packet));

        av_dump_format(
                       format_ctx,
                       best_stream_index,
                       "log.txt",
                       0
               