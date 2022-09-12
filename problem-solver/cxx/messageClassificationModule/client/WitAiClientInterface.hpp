#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace messageClassificationModule
{
class WitAiClientInterface
{
public:
  virtual json getWitResponse(std::string const & messageText) = 0;

  virtual ~WitAiClientInterface() = default;
};

}  // namespace messageClassificationModule
