#pragma once

#include <string>
#include <curl/curl.h>

namespace messageClassificationModule 
{

class MessageClassificationHelper
{
public:
  std::string unicode_escape_to_utf8(const std::string& input);
};

} // namespace messageClassificationModule
