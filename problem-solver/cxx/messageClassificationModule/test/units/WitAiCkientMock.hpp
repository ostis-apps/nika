#pragma once

#include "client/WitAiClientInterface.hpp"
#include "gmock/gmock.h"

namespace messageClassificationModule
{
class WitAiClientMock : public WitAiClientInterface
{
public:
  MOCK_METHOD(json, getWitResponse, (std::string const & messageText), (override));
};

}  // namespace messageClassificationModule
