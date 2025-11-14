#include <iostream>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

enum class EFormatType 
{
    EFT_INPUT  = 0 ,
    EFT_OUTPUT = 1 ,
};

std::ostream& operator<<(std::ostream& o, AVFormatContext* ctx)
{
    av_dump_format(
                   ctx,
                   0,
                   NULL,
                   static_cast<std::underlying_type_t<EFormatType>>(EFormatType::EFT_INPUT)
                   );
    return o; 
};

std::ostream& operator<<(std::ostream& o, AVStream* stream)
{
    if(stream == nullptr) 
        return o; 
    fprintf(stdout, "%s\n", stream->av_class->class_name);
    return o; 
};

template <typename T>
struct Iterator
{
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t; 
    using value_type        = T; 
    using pointer           = T*;
    using reference         = T&;

private:
    pointer m_ptr;

public:

    reference operator*() const { return *m_ptr; } 
    pointer   operator->() { return m_ptr; } 
    Iterator& operator++() { m_ptr++; return *this ; }
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }


    Iterator(pointer ptr) : m_ptr{ptr} {} 

    friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr ; }
    friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr ; }

};

int main (int argc, char *argv[]) 
{

    const AVCodec* codec; 
    AVCodecContext* cctx = NULL ; 
    AVCodecParserContext* pctx = NULL ; 

    AVFormatContext* fmt_ctx = NULL; 
    AVPacket* pkt = av_packet_alloc(); 
    const char* fname{"input.mp4"}; 

    if(avformat_open_input(&fmt_ctx, fname, NULL, NULL) < 0)
    {
        std::cout << "Could not initialize context" << std::endl;
        avformat_close_input(&fmt_ctx);
        std::exit(EXIT_FAILURE);
    }

    if(avformat_find_stream_info(fmt_ctx, NULL) < 0)
    {
        std::cout << "Could not initialize context" << std::endl;
        avformat_close_input(&fmt_ctx);
        std::exit(EXIT_FAILURE);
    }

    /*
    for(std::size_t i = 0 ; i < fmt_ctx->nb_streams ; i++)
    {
        AVStream* stream{ fmt_ctx->streams[i] };
        auto s  = av_get_media_type_string(stream->codecpar->codec_type);
        std::cout << s << std::endl;
    }
    */
    
    av_read_frame(fmt_ctx, pkt);
    std::cout << pkt->stream_index << std::endl;

    // while(av_read_frame(fmt_ctx, pkt))
    // {
    //     std::cout << pkt->stream_index << std::endl;
    // }

    av_packet_free(&pkt);
    return 0;
}
