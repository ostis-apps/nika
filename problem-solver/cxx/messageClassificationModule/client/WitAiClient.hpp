#pragma once

#include "client/ClientInterface.hpp"

namespace messageClassificationModule
{
class WitAiClient : public ClientInterface
{
public:
  WitAiClient();

  json getResponse(std::string const & messageText) override;

  ~WitAiClient() override = default;

protected:
  std::string witAiServerToken;

  std::string witAiUrl;
};

}  // namespace messageClassificationModule
