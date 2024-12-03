#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace messageClassificationModule
{
class ClientInterface
{
public:
  virtual json getResponse(std::string const & messageText) = 0;

  virtual ~ClientInterface() = default;
};

}  // namespace messageClassificationModule
