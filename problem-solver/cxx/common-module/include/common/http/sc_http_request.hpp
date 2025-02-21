#pragma once

#include "sc_http_response.hpp"

#include <string>
#include <vector>

using Params = std::map<std::string, std::string>;

class ScHttpRequest
{
public:
  enum class Type : uint8_t
  {
    GET,
    POST,
    PUT
  };

  explicit ScHttpRequest(std::string const & url, Params const & params);
  ~ScHttpRequest();

  ScHttpResponsePtr Perform();

  void SetURL(std::string const & url);
  void SetType(Type type);
  // Set data for a POST and PUT requests
  void SetData(Params const & data);

  void SetHeaders(std::vector<std::string> && headers);
  void AddHeader(std::string const & header);

private:
  void * m_handle;
  Type m_type;
  std::string m_data;
  std::vector<std::string> m_headers;
};
