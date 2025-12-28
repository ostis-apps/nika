#pragma once

#include "sc-memory/sc_addr.hpp"

#include "classifier/RasaMessageTopicClassifier.hpp"

#include "manager/MessageTopicClassificationManagerInterface.hpp"

namespace messageClassificationModule
{
class RasaMessageTopicClassificationManager : public MessageTopicClassificationManagerInterface
{
public:
  explicit RasaMessageTopicClassificationManager(ScAgentContext * context, utils::ScLogger * logger);

  ScAddrVector manage(ScAddrVector const & processParameters) const override;

protected:
  std::unique_ptr<RasaMessageTopicClassifier> classifier;
};

}  // namespace messageClassificationModule