#include "RasaClient.hpp"

#include <iostream>
#include <sstream>
#include <curl/curl.h>

using namespace messageClassificationModule;

RasaClient::RasaClient() {
    apiUrl = "http://localhost:5005/model/parse";
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
}

RasaClient::~RasaClient(){
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

json RasaClient::getResponse(const std::string& messageText){
    if (!curl) {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());

    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    std::string jsonData = "{\"text\":\"" + messageText + "\"}";
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json; charset=UTF-8");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    readBuffer.clear();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return "";
    }

    return json::parse(helper.unicode_escape_to_utf8(readBuffer));
}

size_t RasaClient::writeCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}