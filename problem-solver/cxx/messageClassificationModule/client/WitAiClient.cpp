#include "WitAiClient.hpp"

#include "sc-config/sc_config.hpp"
#include "sc-memory/utils/sc_log.hpp"

#include "http/sc_http_request.hpp"

messageClassificationModule::WitAiClient::WitAiClient()
{
  ScConfig config{MESSAGE_CLASSIFICATION_CONFIG_PATH};
  ScConfigGroup group{config["wit-ai"]};
  witAiServerToken = group["server_token"];
  witAiUrl = group["url"];
}

json messageClassificationModule::WitAiClient::getResponse(std::string const & messageText)
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
