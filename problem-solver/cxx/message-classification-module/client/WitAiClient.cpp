#include "WitAiClient.hpp"

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/utils/sc_logger.hpp>

#include <common/http/sc_http_request.hpp>
#include <sc-config/sc_config.hpp>

messageClassificationModule::WitAiClient::WitAiClient()
{
  ScConfig config{ScMemory::ms_configPath};
  ScConfigGroup group{config["wit-ai"]};
  witAiServerToken = group["server_token"];
  witAiUrl = group["url"];
}

json messageClassificationModule::WitAiClient::getWitResponse(std::string const & messageText)
{
  json jsonResponse;
  try
  {
    ScHttpRequest request{witAiUrl, {{"q", messageText}}};
    request.SetType(ScHttpRequest::Type::GET);
    request.AddHeader("Authorization: Bearer " + witAiServerToken);
    ScHttpResponsePtr const response = request.Perform();

    jsonResponse = json::parse(response->GetData());
  }
  catch (...)
  {
    SC_LOG_ERROR("WitAiClient: Internet connection error.");
  }

  return jsonResponse;
}
