#pragma once

#include <sc-memory/sc_agent_context.hpp>

#include "client/WitAiClientInterface.hpp"
#include <common/searcher/MessageSearcher.hpp>

namespace messageClassificationModule
{
class MessageTopicClassifier
{
public:
  explicit MessageTopicClassifier(ScAgentContext * context, std::shared_ptr<WitAiClientInterface> const & client);

  ScAddrVector classifyMessage(ScAddr const & messageAddr);

protected:
  ScAgentContext * context;

  ScAddrVector relationsToFindEntity{ScKeynodes::nrel_main_idtf, ScKeynodes::nrel_idtf};

  std::unique_ptr<commonModule::MessageSearcher> messageSearcher;

  std::shared_ptr<WitAiClientInterface> client;

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
