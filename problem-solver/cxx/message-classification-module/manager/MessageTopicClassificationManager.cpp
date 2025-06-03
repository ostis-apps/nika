#include "manager/MessageTopicClassificationManager.hpp"

#include "client/WitAiClient.hpp"

namespace messageClassificationModule
{

MessageTopicClassificationManager::MessageTopicClassificationManager(ScAgentContext * context, utils::ScLogger * logger)
  : classifier(std::make_unique<MessageTopicClassifier>(context, logger, std::make_shared<WitAiClient>(logger)))
{
}

ScAddrVector MessageTopicClassificationManager::manage(ScAddrVector const & processParameters) const
{
  ScAddr messageAddr = processParameters.at(0);
  ScAddrVector answerElements = classifier->classifyMessage(messageAddr);

  return answerElements;
}

}  // namespace messageClassificationModule
