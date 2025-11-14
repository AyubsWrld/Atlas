#pragma once
#include "audiotypes.hpp"

class UAudioLoader
{
  FAudioResource m_file;

  UAudioLoader() = delete;
  UAudioLoader(const UAudioLoader&) = delete;
  UAudioLoader operator=(const UAudioLoader&) = delete;

public:

  UAudioLoader(const std::string& filename);
  ~UAudioLoader();

};
