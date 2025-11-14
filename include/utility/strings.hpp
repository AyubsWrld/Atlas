#pragma once
#include <algorithm>
#include <string>

inline std::optional<std::pair<std::string,std::string>> ParseFilename(const std::string& filename)
{
  std::ptrdiff_t idx; 
  std::string::const_iterator cbegin( filename.begin() );
  std::string::const_iterator cend( filename.end() );
  return std::nullopt;
}
