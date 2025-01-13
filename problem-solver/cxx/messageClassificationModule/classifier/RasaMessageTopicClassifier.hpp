#pragma once

#include "sc-memory/sc_agent_context.hpp"

#include "client/ClientInterface.hpp"
#include "client/RasaClient.hpp"

#include "searcher/MessageSearcher.hpp"

#include "keynodes/MessageClassificationKeynodes.hpp"

namespace messageClassificationModule
{

class RasaMessageTopicClassifier
{
public:
  explicit RasaMessageTopicClassifier(ScAgentContext * context, std::shared_ptr<RasaClient> const & client);

  ScAddrVector classifyMessage(ScAddr const & messageAddr);
protected:
  ScAgentContext * context;

  ScAddrVector relationsToFindEntity{ScKeynodes::nrel_main_idtf, ScKeynodes::nrel_idtf};

  std::unique_ptr<dialogControlModule::MessageSearcher> messageSearcher;

  std::shared_ptr<ClientInterface> client;

  std::string getMessageText(ScAddr const & messageAddr);

  ScAddrVector getMessageIntentClass(ScAddr const & messageAddr, json const & rasaResponse);

  static std::string getMessageIntent(json const & rasaResponse);

  std::vector<std::string> getRasaIdtfs(ScAddr const & messageClass);

  ScAddrVector getMessageEntity(ScAddr const & messageAddr, json const & rasaResponse);

  static std::vector<json> getMessageEntities(json const & rasaResponse);

  static void buildEntityTemplate(ScTemplate & entityTemplate, ScAddr const & possibleEntityClass);

  ScAddrVector processEntities(
      ScIterator3Ptr & possibleEntityIterator,
      std::vector<json> const & messageEntity,
      ScAddrVector & messageEntitiesElements,
      ScAddr const & messageAddr);
};

}  // namespace messageClassificationModule