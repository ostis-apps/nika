#include "common/http/sc_http_response.hpp"

#include <curl/curl.h>
#include <utility>

ScHttpResponse::ScHttpResponse(std::string data, uint8_t resultCode)
  : m_data(std::move(data))
  , m_resultCode(resultCode)
{
}

std::string const & ScHttpResponse::GetData() const
{
  return m_data;
}

std::string ScHttpResponse::GetResultCodeString() const
{
  return curl_easy_strerror((CURLcode)m_resultCode);
}

sc_bool ScHttpResponse::IsSuccess() const
{
  return ((CURLcode)m_resultCode == CURLE_OK);
}
