#pragma once
#include <fstream>

enum class EAudioFileFormats
{
  EAFF_UNDEF,
  EAFF_WAV,
  EAFF_MP3,
  EAFF_OGG,
  EAFF_MAX
};


struct FAudioResource 
{
  EAudioFileFormats FileFormat;
  std::ifstream FHandle; 
};
