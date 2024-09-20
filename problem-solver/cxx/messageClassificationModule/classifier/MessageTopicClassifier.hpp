#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_agent.hpp"

#include "client/WitAiClient.hpp"
#include "client/WitAiClientInterface.hpp"
#include "searcher/MessageSearcher.hpp"
namespace messageClassificationModule
{
class MessageTopicClassifier
{
public:
  explicit MessageTopicClassifier(ScAgentContext * context, WitAiClientInterface * client);

  ScAddrVector classifyMessage(ScAddr const & messageAddr);

protected:
  ScAgentContext * context;

  ScAddrVector relationsToFindEntity{ScKeynodes::nrel_main_idtf, ScKeynodes::nrel_idtf};

  std::unique_ptr<dialogControlModule::MessageSearcher> messageSearcher;

  std::unique_ptr<WitAiClientInterface> client;

  std::string getMessageText(ScAddr const & messageAddr);

  ScAddrVector getMessageIntentClass(ScAddr const & messageAddr, json const & witResponse);

  static std::string getMessageIntent(json const & witResponse);

  std::vector<std::string> getWitAiIdtfs(ScAddr const & messageClass);

  ScAddrVector getMessageTraitClass(ScAddr const & messageClass, json const & witResponse);

  static json getMessageTrait(json const & witResponse);

  static void buildTraitTemplate(ScTemplate & traitTemplate, ScAddr const & possibleMessageCLass);

  ScAddrVector processTraits(
      ScIterator3Ptr & possibleTraitIterator,
      json const & messageTrait,
      ScAddrVector & messageTraitClassElements,
      ScAddr const & messageAddr);

  ScAddrVector getMessageEntity(ScAddr const & messageAddr, json const & witResponse);

  static json getMessageEntities(json const & witResponse);

  static void buildEntityTemplate(ScTemplate & entityTemplate, ScAddr const & possibleEntityClass);

  ScAddrVector processEntities(
      ScIterator3Ptr & possibleEntityIterator,
      json const & messageEntity,
      ScAddrVector & messageEntitiesElements,
      ScAddr const & messageAddr);
};

}  // namespace messageClassificationModule
