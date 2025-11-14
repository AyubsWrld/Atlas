#include <iostream>
#include <fstream>
#include <concepts>

#define BUFFERSIZE 4096

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

template <typename T>
concept Callable = requires (T a) { std::is_invocable_v<T> ; }; 

template <Callable T>
struct defer
{
    T callable;
    ~defer()
    {
        callable();
    }
};

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

static void decode(AVCodecContext* ctx, AVPacket* pkt, AVFrame* frame)
{

}


int main (int argc, char *argv[]) 
{
    FILE* fin  { fopen("input.mp3", "rb") };
    FILE* fout { fopen("onput.mp3", "wb") }; 

    const AVCodec*        codec        = avcodec_find_decoder(AV_CODEC_ID_MP3); 
    AVCodecContext*       codecctx     = avcodec_alloc_context3(codec); 
    AVCodecParserContext* pctx         = av_parser_init(codec->id); // This calls parser alloc && binds const reference to AVCodecParser. 


    AVPacket*             pkt          = av_packet_alloc();
    AVFrame*              frame        = av_frame_alloc();

    int                   read        {}; 
    int                   ret         {}; 
    uint8_t               buffer[BUFFERSIZE];
    uint8_t*              data{buffer};

    auto free_resources = [&](){
        std::cout << "Freeing resources" << std::endl;
        av_parser_close(pctx);
        avcodec_free_context(&codecctx);
        av_packet_free(&pkt);
        av_frame_free(&frame);
    };

    if(!(codec && codecctx && pctx))
    {
        std::cout << "Initialization Error" << std::endl;
        free_resources();
        std::exit(EXIT_FAILURE);
    }

    if(!(frame && pkt))
    {
        std::cout << "Packet/Frame Allocation Error" << std::endl;
        free_resources();
        std::exit(EXIT_FAILURE);
    }

    /* What does a closed vs open codec state imply */
    
    if(avcodec_open2(codecctx,codec,NULL) != 0)
    {
        av_parser_close(pctx);
        avcodec_free_context(&codecctx);
        std::exit(EXIT_FAILURE);
    }

    read = fread(buffer,1,BUFFERSIZE,fin);
    while(read > 0)
    {
        ret = av_parser_parse2(
                               pctx,
                               codecctx,
                               &pkt->data,
                               &pkt->size,
                               data,
                               BUFFERSIZE,
                               AV_NOPTS_VALUE,
                               AV_NOPTS_VALUE,
                               0
                               );
        data += ret; 
        read -= ret; 

        if (ret < 0) {
            fprintf(stderr, "Error while parsing\n");
            exit(1);
        }

        /* Shift index within the buffer */
        data      += ret;    

        /* Shift index within the amount of data that needs to be read */
        read      -= ret;  // 
 
        /* We've written a value to the packet */
        std::cout << pkt << std::endl;
    }
}
