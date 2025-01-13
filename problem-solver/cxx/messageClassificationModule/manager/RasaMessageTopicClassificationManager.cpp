#include "manager/RasaMessageTopicClassificationManager.hpp"

#include "client/RasaClient.hpp"

namespace messageClassificationModule
{
RasaMessageTopicClassificationManager::RasaMessageTopicClassificationManager(ScAgentContext * context)
  : classifier(std::make_unique<RasaMessageTopicClassifier>(context, std::make_shared<RasaClient>()))
{
}

ScAddrVector RasaMessageTopicClassificationManager::manage(ScAddrVector const & processParameters) const
{
  ScAddr messageAddr = processParameters.at(0);
  ScAddrVector answerElements = classifier->classifyMessage(messageAddr);

  return answerElements;
}

}  // namespace messageClassificationModule
