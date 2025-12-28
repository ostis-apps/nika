#include "manager/WitMessageTopicClassificationManager.hpp"

#include "client/WitAiClient.hpp"

namespace messageClassificationModule
{

WitMessageTopicClassificationManager::WitMessageTopicClassificationManager(ScAgentContext * context, utils::ScLogger * logger)
  : classifier(std::make_unique<WitMessageTopicClassifier>(context, logger, std::make_shared<WitAiClient>(logger)))
{
}

ScAddrVector WitMessageTopicClassificationManager::manage(ScAddrVector const & processParameters) const
{
  ScAddr messageAddr = processParameters.at(0);
  ScAddrVector answerElements = classifier->classifyMessage(messageAddr);

  return answerElements;
}

}  // namespace messageClassificationModule
