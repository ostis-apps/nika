#pragma once

#include <gmock/gmock.h>

#include "client/ClientInterface.hpp"

namespace messageClassificationModule
{
class WitAiClientMock : public ClientInterface
{
public:
  MOCK_METHOD(json, getResponse, (std::string const & messageText), (override));
};

}  // namespace messageClassificationModule
