/*
 *  @file pmc.hpp
 *
 *  @brief Declares the logic associated with the task of converting a raw binary audio stream into Pulse-Code Modulation Data for processing.
 *  
 *  This header defines the interface for the MyClass class, which provides
 *  functionalities for creating, manipulating, and destroying widget instances.
 *  It includes methods for adding, removing, and retrieving widgets.
 *
 *  @author Ayub Mohamed
 *
 *  @date 2025-11-13
 *
 */

#pragma once

extern "C"
{
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libavutil/avutil.h>
#include<libavutil/timestamp.h>
}

#include <iostream>
#include <utility>
#include <expected>
#include <format>
#include <cassert>


namespace Atlas
{

    /* Unnamed enumerator for readability */ 
    enum : uint8_t { VALID_STREAM_INDEX = 0 }; 

/*
    #FIXME:         Extrenous and potentially unsafe use of pointers. ownership is unclear.

    #NOTE:          Currently this routine is a wrapper around 4 subroutines that could fail.
                    I currently believe that this is fine because should any of these routines
                    fail the program cannot continue and should exit gracefully. 

    #NOTE:          This routine should itself not exit as there is cleanup that the caller must 
                    do. 

    #NOTE:          Fixed Segfault caused by decoder_ctx being in an invalid state with call 
                    to avformat_allocate_context3 aftter the codec has been initiliaze. 
                    
    
    @purpose        Prepares a Decoder and Decoder Context for decompressing the bestfit stream
                    (The best stream is determined according to various heuristics as the most likely
                    to be what the user expects.) picked from the AVFormatContext's streams of type 
                    AVMediaType. The main purpose of this routine is to provide as much context to the
                    decoding software so that it is equipped for operating on the best fit stream should
                    it be found. 

    @param:         [in]                  const AVCodec**       Decoder which will prepared for decoding bestfit stream if found based on the streams CodecID.

    @param:         [in]                  AVFormatContext*      Reference to format context which houses the individual streams . 

    @param:         [in]                  AVCodecContext**      DecoderContext which will be prepared for decoding bestfit stream if found based on the streams CodecID.
                                                                Owning Pointer the is altered within the context of this function.

    @param:         [in]                  AVMediaType           Mediatype to look for the bestfit stream for. 


                                    return

    @code:          int                         Index of bestfit stream ( Non-negative value on success ) 

    @code:          int                         Value < 0 on failure. 


    @notes:         This is effectively a wrapper around av_find_best_stream ( defined in avformat.h )
                    and has the side effect of writing to the const AVCodec* should the best fit stream 
                    be located. 

    @notes:         A successive call to avcodec_find_decoder is superfluous as av_find_best_stream internally
                    binds a decoder capable of handling the best fit stream in the case that it is found. 

    @notes:         This routine assumes that memory for the decoder context (CodecContext) has been allocated prior 
                    to invoking. Will fail ( with -1 ) early should this not be the case.
                    

*/
    [[nodiscard]]
    int PrepareDecoderForStream(
                                const AVCodec** codec,
                                AVFormatContext* format_context,
                                AVCodecContext** decoder_ctx,
                                AVMediaType mediatype
                                );


/*
 
    @purpose        Opens a file using the name passed in as an argument and writes raw PCM data to a
                    buffer and returns a pointer to the buffer containing the PCM data.
                    

    @param:         [in]                  const AVCodec**       Decoder which will prepared for decoding bestfit stream if found based on the streams CodecID.


                                    return

    @code:          std::span<uint8_t>          Pointer to underlying buffer used to store the PCM data. 

    @code:          int                         Value < 0 on failure. 


    @notes:         This is effectively a wrapper around av_find_best_stream ( defined in avformat.h )
                    and has the side effect of writing to the const AVCodec* should the best fit stream 
                    be located. 

    @notes:         A successive call to avcodec_find_decoder is superfluous as av_find_best_stream internally
                    binds a decoder capable of handling the best fit stream in the case that it is found. 

    @notes:         This routine assumes that memory for the decoder context has been allocated prior to invoking.
                    Will fail early should this not be the case.
                    

*/

    void SplitAudioStream(const char* file = NULL);

/*
    @purpose        Opens a file using the name passed in as an argument and writes raw PCM data to a
                    buffer and returns a pointer to the buffer containing the PCM data.
                    

    @param:         [in]                  const AVCodec**       Decoder which will prepared for decoding bestfit stream if found based on the streams CodecID.


                                    return

    @code:          std::span<uint8_t>          Pointer to underlying buffer used to store the PCM data. 

    @code:          int                         Value < 0 on failure. 


    @notes:         This is effectively a wrapper around av_find_best_stream ( defined in avformat.h )
                    and has the side effect of writing to the const AVCodec* should the best fit stream 
                    be located. 

    @notes:         A successive call to avcodec_find_decoder is superfluous as av_find_best_stream internally
                    binds a decoder capable of handling the best fit stream in the case that it is found. 

    @notes:         This routine assumes that memory for the decoder context has been allocated prior to invoking.
                    Will fail early should this not be the case.
                    

*/
    void ReadAudioStream(AVFormatContext* format_ctx, AVCodecContext* decoder_ctx, int best_stream_index);

    /*

    @purpose        Returns the size in kilobytes needed to store the uncompressed audiofile 
                    
    @param:         [in]                const AVFormatContext*      FormatContext used to store the audiofile. 


                                    return

    @code:          std::span<uint8_t>          Pointer to underlying buffer used to store the PCM data. 

    @code:          int                         Value < 0 on failure. 


    @notes:         This routine assumes that all the necessary steps for priming the foramt context have been applied
                    ( avformat_open_input &avformat_find_stream_info) prior to passing it otherwise the return value 
                    might not be accured.

    @notes:         Currently no error handling is done this is simply a side effectless routine. 
                    

*/
    [[nodiscard]] constexpr std::size_t GetAudioFileSize(const AVFormatContext* codec) noexcept;

/*

@purpose        Decodes an audio file packet into it's equivalent uncompressed frame.
                
@param:         [in]                const AVCodecContext*       FormatContext used to decode packet. 

@param:         [in]                AVPacket*                   Packet containing uncompressed audio data. 

*/

    void DecodeAudioPacket(AVCodecContext* decoder_ctx, AVPacket* packet, FILE* output);


/*

@purpose        OO Utility for printing out AVFrame
                
*/
    std::ostream& operator<<(std::ostream& o, AVFrame* frame);

/*

@purpose        OO Utility for printing out AVFrameBufferRef
                
*/
    std::ostream& operator<<(std::ostream& o, AVBufferRef* buf);
}

