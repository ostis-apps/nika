#include "manager/MessageTopicClassificationManager.hpp"

#include "client/WitAiClient.hpp"

namespace messageClassificationModule
{
MessageTopicClassificationManager::MessageTopicClassificationManager(ScAgentContext * context)
  : classifier(std::make_unique<MessageTopicClassifier>(context, std::make_shared<WitAiClient>()))
{
}

ScAddrVector MessageTopicClassificationManager::manage(ScAddrVector const & processParameters) const
{
  ScAddr messageAddr = processParameters.at(0);
  ScAddrVector answerElements = classifier->classifyMessage(messageAddr);

  return answerElements;
}

}  // namespace messageClassificationModule
