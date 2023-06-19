#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

#include "client/WitAiClient.hpp"
#include "client/WitAiClientInterface.hpp"
#include "searcher/MessageSearcher.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

namespace messageClassificationModule
{
class MessageTopicClassifier
{
public:
  explicit MessageTopicClassifier(ScMemoryContext * context, WitAiClientInterface * client);

  ScAddrVector classifyMessage(ScAddr const & messageAddr);

protected:
  ScMemoryContext * context;

  ScAddrVector relationsToFindEntity {scAgentsCommon::CoreKeynodes::nrel_main_idtf, scAgentsCommon::CoreKeynodes::nrel_idtf};

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
