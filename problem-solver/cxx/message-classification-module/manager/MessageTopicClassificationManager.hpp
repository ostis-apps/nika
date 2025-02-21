#pragma once

#include <sc-memory/sc_addr.hpp>

#include "classifier/MessageTopicClassifier.hpp"

namespace messageClassificationModule
{
class MessageTopicClassificationManager
{
public:
  explicit MessageTopicClassificationManager(ScAgentContext * context);

  ScAddrVector manage(ScAddrVector const & processParameters) const;

protected:
  std::unique_ptr<MessageTopicClassifier> classifier;
};

}  // namespace messageClassificationModule
