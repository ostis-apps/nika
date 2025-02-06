#include "common/http/sc_http_request.hpp"

#include <sc-memory/sc_debug.hpp>

#include <curl/curl.h>

namespace
{
size_t CurlWrite_CallbackFunc_StdString(void * contents, size_t size, size_t nmemb, std::string * s)
{
  size_t newLength = size * nmemb;
  size_t oldLength = s->size();

  try
  {
    s->resize(oldLength + newLength);
  }
  catch (std::bad_alloc &)
  {
    // handle memory problem
    return 0;
  }

  std::copy((sc_char *)contents, (sc_char *)contents + newLength, s->begin() + oldLength);
  return size * nmemb;
}

std::string ParamsToString(Params const & params)
{
  if (params.empty())
    return "";

  auto pb = params.cbegin(), pe = params.cend();
  std::stringstream data;
  data << "?" << pb->first << "=" << curl_escape(pb->second.c_str(), (sc_int)(pb->second.size()));
  ++pb;

  if (pb == pe)
    return data.str();

  for (; pb != pe; ++pb)
    data << "&" << pb->first << "=" << curl_escape(pb->second.c_str(), (sc_int)(pb->second.size()));

  return data.str();
}

}  // namespace

ScHttpRequest::ScHttpRequest(std::string const & url, Params const & params)
  : m_handle(nullptr)
  , m_type(Type::GET)
{
  m_handle = (void *)curl_easy_init();
  if (!url.empty())
  {
    std::string const & paramsStr = ParamsToString(params);
    std::string const path = url + paramsStr;
    SetURL(path);
  }
}

ScHttpRequest::~ScHttpRequest()
{
  curl_easy_cleanup((CURL *)m_handle);
}

ScHttpResponsePtr ScHttpRequest::Perform()
{
  CURL * curl = (CURL *)m_handle;

  if (m_type == Type::POST)
  {
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_data.c_str());
  }
  else if (m_type == Type::PUT)
  {
    SC_NOT_IMPLEMENTED("Type::PUT not implemented yet");
  }

  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  // prepare headers
  curl_slist * headers = nullptr;
  for (std::string const & h : m_headers)
  {
    headers = curl_slist_append(headers, h.c_str());
  }
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  std::string responseString;
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

  // perform request
  CURLcode const resultCode = curl_easy_perform(curl);
  curl_slist_free_all(headers);

  return std::make_shared<ScHttpResponse>(responseString, (uint8_t)resultCode);
}

void ScHttpRequest::SetURL(std::string const & url)
{
  curl_easy_setopt((CURL *)m_handle, CURLOPT_URL, url.c_str());
}

void ScHttpRequest::SetType(Type type)
{
  m_type = type;
}

void ScHttpRequest::SetData(Params const & data)
{
  m_data = ParamsToString(data);
}

void ScHttpRequest::SetHeaders(std::vector<std::string> && headers)
{
  m_headers = headers;
}

void ScHttpRequest::AddHeader(std::string const & header)
{
  m_headers.push_back(header);
}
