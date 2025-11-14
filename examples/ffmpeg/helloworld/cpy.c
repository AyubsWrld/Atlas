#include <libavformat/avformat.h>

int main(int argc, char *argv[]) {
    AVFormatContext *fmt_ctx = NULL;
    const char *filename = "input.mp4";

    /* Initializes FormatContext */ 
    /* 		1). Calls "ctor" for AVFormat Context & Initializes internal stat*/
    /* 		2). Returns 0 is successful and a value < 0 if not. */
    if (avformat_open_input(&fmt_ctx, filename, NULL, NULL) < 0) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return -1;
    }

    /* Bulk of the work is done here */ 
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream info\n");
        avformat_close_input(&fmt_ctx);
        return -1;
    }

    av_dump_format(fmt_ctx, 0, filename, 0);

    avformat_close_input(&fmt_ctx);
    return 0;
}
