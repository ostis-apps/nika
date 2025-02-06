#pragma once

#include <sc-memory/sc_utils.hpp>

#include <cstdint>
#include <string>

class ScHttpResponse
{
protected:
  friend class ScHttpRequest;

public:
  std::string const & GetData() const;
  std::string GetResultCodeString() const;

  sc_bool IsSuccess() const;

  ScHttpResponse(std::string data, uint8_t resultCode);

private:
  std::string m_data;
  uint8_t m_resultCode;
};

SHARED_PTR_TYPE(ScHttpResponse);
