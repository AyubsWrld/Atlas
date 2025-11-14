#include <iostream>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
}

std::ostream& operator<<(std::ostream& o, AVPacket* p)
{
    if(!p) return o; 
    fprintf(
            stdout,
            "Packet: (Size: %d, Stream Index %d, Duration: %lu, Position: %lu",
            p->size,
            p->stream_index,
            p->duration,
            p->pos
            );
    return o; 

}

int main (int argc, char *argv[]) 
{
    AVFormatContext* formatctx = NULL; 
    AVPacket*        pkt       = NULL; 

    /* avformat_open_input calls ctor itself */
    formatctx = avformat_alloc_context(); 
    pkt       = av_packet_alloc();  // Call to this is unneeded per av_read_frame as it initializes the packet itself.



    if(avformat_open_input(&formatctx, "input.mp4",NULL,NULL) != 0)
    {
        std::cout << "Could not open destination" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    const AVCodec*   codec     = avcodec_find_encoder(AVCodecID::AV_CODEC_ID_AAC);

    if(!codec)
        std::cout << "Could not find codec";

    auto _cleanup = [&](){
        av_packet_free(&pkt);
        if(formatctx)
            avformat_free_context(formatctx);
    };
   

    /* Returns 0 if a value from the byte stream has been successfully loaded into the packet */ 
    while(av_read_frame(formatctx, pkt) == 0)
    {
        // std::cout << pkt << std::endl;
    }

    _cleanup();
    return 0;
}
