#include <iostream>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}



std::ostream& operator<<(std::ostream& o, AVFormatContext* fmt_ctx)
{
    if(!fmt_ctx) return o; 
    fprintf(
            stdout,
            "Format Context:\n\t(Number of Streams): %d\n\t(Number of Stream Groups): %d\n\t(URL): %s\n\t(Duration): %lu\n\t(Bitrate): %lu\n\t(Packet size): %d",
            fmt_ctx->nb_streams,
            fmt_ctx->nb_stream_groups,
            fmt_ctx->url,
            fmt_ctx->duration,
            fmt_ctx->bit_rate,
            fmt_ctx->packet_size
            );
    return o;
}

std::ostream& operator<<(std::ostream& o, const AVCodec* const codec)
{
    if(!codec) return o; 
    return o;
}


/*
    #FIXME:         Extrenous and potentially unsafe use of pointers. ownership is unclear.
    
    @purpose        Prepares a Decoder and Decoder Context for decompressing the bestfit stream
                    picked from the AVFormatContext's streams of type AVMediaType. The main purpose
                    of this routine is to provide as much context to the decoding software so that
                    it is equipped for operating on the best fit stream should it be found. 

    @param:         [in]                  const AVCodec**       Decoder which will prepared for decoding bestfit stream if found based on the streams CodecID.

    @param:         [in]                  AVCodecContext**      DecoderContext which will be prepared for decoding bestfit stream if found based on the streams CodecID.

    @param:         [in]                  AVFormatContext*      Reference to format context which houses the individual streams . 

    @param:         [in]                  AVMediaType           Mediatype to look for the bestfit stream for. 


                                       return

    @code:          int                         Index of bestfit stream ( Non-negative value on success ) 

    @code:          int                         Value < 0 on failure. 


    @notes:         This is effectively a wrapper around av_find_best_stream ( defined in avformat.h )
                    and has the side effect of writing to the const AVCodec* should the best fit stream 
                    be located. 

    @notes:         A successive call to avcodec_find_decoder is superfluous as av_find_best_stream internally
                    binds a decoder capable of handling the best fit stream in the case that it is found. 

*/


[[nodiscard]] int prepare_codec_context(
                            const AVCodec** dec_codec,
                            AVCodecContext** decodercontext,
                            AVFormatContext* fmt_ctx,
                            AVMediaType mtype
                          )
{
    int ret = -1; 
    AVStream* best_fit_stream = NULL ;

    /* 
     * Find the most suitable stream of based on bitrate, multiframe, and disposition.
     * Limits candidates to streams of the same type as AVMediaType. passed as argument.
     * returns index of stream on success ( Non-negative value )
     * returns a negative value otherwise
     * Sets decoder as a side effect
    */
    
    ret = av_find_best_stream(
                              fmt_ctx, 
                              mtype,   
                              -1,
                              -1,
                              dec_codec,
                              0
                            );
    if(ret < 0)
    {
        std::cout << "(" << __FILE__ << "): "<< "Failed to find optimal stream: " << __LINE__ << std::endl;
        return ret; 
    }

    /* Best fit stream was found and index is stored in ret */ 
    best_fit_stream = fmt_ctx->streams[ret];

    /* Prepare Decoder Context */ 
    *decodercontext = avcodec_alloc_context3(*dec_codec);

    /* Copy codec parameters from best fit stream into our decoder context ( More information ) */ 
    if(avcodec_parameters_to_context(*decodercontext, best_fit_stream->codecpar) < 0) 
    {
        std::cout << "(" << __FILE__ << "): "<< "An error occured while trying to copy codec parameters from best fit stream: " << __LINE__ << std::endl;
        return ret; 
    }

    /* Is this needed ? */ 
    
    if(avcodec_open2(*decodercontext, *dec_codec, NULL ) != 0)
    {
        std::cout << "(" << __FILE__ << "): "<< "An error occured while trying to initialize the codec to use the given context: " << __LINE__ << std::endl;
        return ret; 
    }
    return ret; 
}

[[nodiscard]] int decode_audio_packet(AVCodecContext* decoder, AVFrame* frame)
{
    int ret; 
    av_frame_unref(frame);
    return -1;
}


int main (int argc, char *argv[]) 
{
    /* Write contract */

    int streamidx      = -1 ; 
    int bytes_recieved = -1 ; 
    /* These are for decoding the streams, and as such require context for decoding the streams */ 
    const AVCodec* AudioCodec = NULL; 
    const AVCodec* AudioEncoder = NULL; 
    const AVCodec* VideoCodec = NULL; 


    /* Active decoding stream session */
    AVCodecContext*  AudioCodecContext = NULL; 
    AVCodecContext*  VideoCodecContext = NULL; 


    AVCodecContext*  AudioEncoderContext = NULL; 

    AVPacket*        pkt               = NULL; 
    AVFrame*         frame             = NULL; 
    AVFormatContext* FormatContext     = NULL; 
    AVFormatContext* OutputFormatContext = NULL; 
    AVOutputFormat*  Outputcontext      = NULL; 

    /* Setup FormatContext for referencing the resource, this is where our bytes comes from */
 
    /* Call to avformat_open_input allocates space */
  
    if(avformat_open_input(&FormatContext, "input.mp4", NULL, NULL) != 0)
    {
        std::cout << "Could not open format" << std::endl;
        std::exit(EXIT_FAILURE);
    }


    if(avformat_find_stream_info(FormatContext, NULL) < 0)
    {
        std::cout << "Failed to populate stream info" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    /* Setup CodecContext */

    streamidx = prepare_codec_context(
                                    &AudioCodec,
                                    &AudioCodecContext,
                                    FormatContext,
                                    AVMediaType::AVMEDIA_TYPE_AUDIO
                                 );

    if(streamidx  < 0)
    {
        std::cout << "(" << __FILE__ << "): "<< "Failed to find best fit stream: " << __LINE__ << std::endl;
        std::exit(EXIT_FAILURE);
    }

    AudioEncoder = avcodec_find_encoder(AudioCodec->id);
    AudioEncoderContext = avcodec_alloc_context3(AudioEncoder);

    avcodec_parameters_to_context(AudioEncoderContext, FormatContext->streams[streamidx]->codecpar);

    pkt   =   av_packet_alloc(); 
    frame =   av_frame_alloc(); 
    if(!pkt || !frame)
    {
        std::cout << "(" << __FILE__ << "): "<< "Failed to allocate resources for data transfer (packet/frame): " << __LINE__ << std::endl;
        std::exit(EXIT_FAILURE);
    }

    while(av_read_frame(FormatContext, pkt) == 0) // Continue to read packets from Context 
    {
        /* Dynamically dispatch based on index */ 
        if(pkt->stream_index == streamidx)
        {
            if(avcodec_send_packet(AudioCodecContext, pkt) == 0)
            {
                int s = decode_audio_packet(AudioCodecContext, frame);
            }
        }
        av_packet_unref(pkt);
    }
    /* Cleanup */

    av_packet_free(&pkt);
    av_frame_free(&frame);

    return 0;
}
