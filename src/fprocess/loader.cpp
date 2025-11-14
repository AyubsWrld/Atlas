#include "loader.hpp"

extern "C" 
{
#include <libavcodec/avcodec.h>
}

UAudioLoader::UAudioLoader(const std::string& filename)
  : m_file(FAudioResource{EAudioFileFormats::EAFF_UNDEF, std::ifstream(filename)})
{
  /* Set file extension type */ 
}
