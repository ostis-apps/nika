#pragma once

#include <sc-memory/utils/sc_logger.hpp>

#include "client/ClientInterface.hpp"

namespace messageClassificationModule
{

class WitAiClient : public ClientInterface
{
public:
  WitAiClient(utils::ScLogger * logger);

  json getResponse(std::string const & messageText) override;

  ~WitAiClient() override = default;

protected:
  utils::ScLogger * logger;
  std::string witAiServerToken;

  std::string witAiUrl;
};

}  // namespace messageClassificationModule
