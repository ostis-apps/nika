#pragma once

#include <string>
#include <curl/curl.h>

#include "helper/MessageClassificationHelper.hpp"
#include <nlohmann/json.hpp>

#include "client/ClientInterface.hpp"

using json = nlohmann::json;

namespace messageClassificationModule 
{
  
  class RasaClient : public ClientInterface
  {
    public:
      RasaClient();
      ~RasaClient() override;
      json getResponse(const std::string& messageText) override;
    private:
      MessageClassificationHelper helper;
      CURL* curl;
      std::string apiUrl;
      std::string readBuffer;

      static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
  };

} // namespace MessageClassificationModule 
