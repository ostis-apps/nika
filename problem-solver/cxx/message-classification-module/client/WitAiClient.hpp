#pragma once

#include <sc-memory/utils/sc_logger.hpp>

#include "client/WitAiClientInterface.hpp"

namespace messageClassificationModule
{

class WitAiClient : public WitAiClientInterface
{
public:
  WitAiClient(utils::ScLogger * logger);

  json getWitResponse(std::string const & messageText) override;

  ~WitAiClient() override = default;

protected:
  utils::ScLogger * logger;
  std::string witAiServerToken;

  std::string witAiUrl;
};

}  // namespace messageClassificationModule
