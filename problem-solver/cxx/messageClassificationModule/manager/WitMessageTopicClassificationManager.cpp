#include "manager/WitMessageTopicClassificationManager.hpp"

#include "client/WitAiClient.hpp"

namespace messageClassificationModule
{
WitMessageTopicClassificationManager::WitMessageTopicClassificationManager(ScAgentContext * context)
  : classifier(std::make_unique<WitMessageTopicClassifier>(context, std::make_shared<WitAiClient>()))
{
}

ScAddrVector WitMessageTopicClassificationManager::manage(ScAddrVector const & processParameters) const
{
  ScAddr messageAddr = processParameters.at(0);
  ScAddrVector answerElements = classifier->classifyMessage(messageAddr);

  return answerElements;
}

}  // namespace messageClassificationModule
