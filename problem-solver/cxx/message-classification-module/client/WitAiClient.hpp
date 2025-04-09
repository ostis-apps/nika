#pragma once

#include "client/WitAiClientInterface.hpp"

namespace messageClassificationModule
{
class WitAiClient : public WitAiClientInterface
{
public:
  WitAiClient();

  json getWitResponse(std::string const & messageText) override;

  ~WitAiClient() override = default;

protected:
  std::string witAiServerToken;

  std::string witAiUrl;
};

}  // namespace messageClassificationModule
