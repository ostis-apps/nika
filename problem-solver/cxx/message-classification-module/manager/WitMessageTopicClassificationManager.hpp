#pragma once

#include <sc-memory/sc_addr.hpp>

#include "classifier/WitMessageTopicClassifier.hpp"
#include "manager/MessageTopicClassificationManagerInterface.hpp"

namespace messageClassificationModule
{

class WitMessageTopicClassificationManager : public MessageTopicClassificationManagerInterface
{
public:
  explicit WitMessageTopicClassificationManager(ScAgentContext * context, utils::ScLogger * logger);

  ScAddrVector manage(ScAddrVector const & processParameters) const;

protected:
  std::unique_ptr<WitMessageTopicClassifier> classifier;
};

}  // namespace messageClassificationModule
