#include "MessageTopicClassifier.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "constants/MessageClassificationConstants.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "searcher/MessageSearcher.hpp"

namespace messageClassificationModule
{
MessageTopicClassifier::MessageTopicClassifier(ScMemoryContext * context, WitAiClientInterface * client)
  : context(context)
  , client(client)
{
  messageSearcher = std::make_unique<dialogControlModule::MessageSearcher>(this->context);
}

ScAddrVector MessageTopicClassifier::classifyMessage(ScAddr const & messageAddr)
{
  ScAddrVector messageClassificationElements = {messageAddr};

  std::string const messageText = getMessageText(messageAddr);

  json const witResponse = client->getWitResponse(messageText);
  SC_LOG_INFO(witResponse);

  ScAddrVector const messageIntentElements = getMessageIntentClass(messageAddr, witResponse);
  messageClassificationElements.insert(
      messageClassificationElements.cend(), messageIntentElements.cbegin(), messageIntentElements.cend());

  ScAddrVector const messageTraitElements = getMessageTraitClass(messageAddr, witResponse);
  messageClassificationElements.insert(
      messageClassificationElements.cend(), messageTraitElements.cbegin(), messageTraitElements.cend());

  ScAddrVector const messageEntitiesElements = getMessageEntity(messageAddr, witResponse);
  messageClassificationElements.insert(
      messageClassificationElements.cend(), messageEntitiesElements.cbegin(), messageEntitiesElements.cend());

  return messageClassificationElements;
}

std::string MessageTopicClassifier::getMessageText(ScAddr const & messageAddr)
{
  ScAddr const messageLink = messageSearcher->getMessageLink(messageAddr);
  if (!messageLink.IsValid())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "MessageTopicClassifier: Message link is not found.");
  }
  return utils::CommonUtils::getLinkContent(context, messageLink);
}

ScAddrVector MessageTopicClassifier::getMessageIntentClass(ScAddr const & messageAddr, json const & witResponse)
{
  ScAddrVector messageIntentCLassElements;

  std::string const messageIntent = getMessageIntent(witResponse);
  if (messageIntent.empty())
  {
    ScAddr const & messageIntentCLassEdge = context->CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        MessageClassificationKeynodes::concept_not_classified_by_intent_message,
        messageAddr);
    messageIntentCLassElements.push_back(MessageClassificationKeynodes::concept_not_classified_by_intent_message);
    messageIntentCLassElements.push_back(messageIntentCLassEdge);
    return messageIntentCLassElements;
  }

  ScIterator3Ptr const possibleIntentIterator = context->Iterator3(
      MessageClassificationKeynodes::concept_intent_possible_class,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConstClass);

  std::vector<std::string> witAiIdtfs;
  ScAddr possibleMessageCLass;
  while (possibleIntentIterator->Next())
  {
    possibleMessageCLass = possibleIntentIterator->Get(2);
    witAiIdtfs = getWitAiIdtfs(possibleMessageCLass);

    for (std::string const & witAiIdtf : witAiIdtfs)
    {
      if (messageIntent == witAiIdtf)
      {
        SC_LOG_DEBUG(
            "MessageTopicClassifier: found " + context->HelperGetSystemIdtf(possibleMessageCLass) + " intent class");
        ScAddr messageIntentCLassEdge =
            context->CreateEdge(ScType::EdgeAccessConstPosPerm, possibleMessageCLass, messageAddr);
        messageIntentCLassElements.push_back(possibleMessageCLass);
        messageIntentCLassElements.push_back(messageIntentCLassEdge);
        return messageIntentCLassElements;
      }
    }
  }

  return messageIntentCLassElements;
}

std::string MessageTopicClassifier::getMessageIntent(json const & witResponse)
{
  std::string messageIntent;
  try
  {
    messageIntent = witResponse.at(WitAiConstants::INTENTS).at(0).at(WitAiConstants::NAME);
  }
  catch (...)
  {
    SC_LOG_WARNING("MessageTopicClassifier: Message intent class is not found.");
  }

  return messageIntent;
}

std::vector<std::string> MessageTopicClassifier::getWitAiIdtfs(ScAddr const & messageClass)
{
  ScAddrVector witAiIdtfAddrs =
      utils::IteratorUtils::getAllByOutRelation(context, messageClass, MessageClassificationKeynodes::nrel_wit_ai_idtf);
  std::vector<std::string> witAiIdtfs;
  for (ScAddr const & witAiIdtfAddr : witAiIdtfAddrs)
  {
    witAiIdtfs.push_back(utils::CommonUtils::getLinkContent(context, witAiIdtfAddr));
  }

  return witAiIdtfs;
}

ScAddrVector MessageTopicClassifier::getMessageTraitClass(ScAddr const & messageAddr, json const & witResponse)
{
  ScAddrVector messageTraitClassElements;

  json const messageTrait = getMessageTrait(witResponse);
  if (messageTrait.empty())
  {
    ScAddr const & messageIntentCLassEdge = context->CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        MessageClassificationKeynodes::concept_not_classified_by_trait_message,
        messageAddr);
    messageTraitClassElements.push_back(MessageClassificationKeynodes::concept_not_classified_by_trait_message);
    messageTraitClassElements.push_back(messageIntentCLassEdge);
    return messageTraitClassElements;
  }

  ScIterator3Ptr possibleTraitIterator = context->Iterator3(
      MessageClassificationKeynodes::concept_trait_possible_class,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConstClass);

  messageTraitClassElements =
      processTraits(possibleTraitIterator, messageTrait, messageTraitClassElements, messageAddr);

  return messageTraitClassElements;
}

json MessageTopicClassifier::getMessageTrait(json const & witResponse)
{
  json messageIntent;
  try
  {
    messageIntent = witResponse.at(WitAiConstants::TRAITS);
  }
  catch (...)
  {
    SC_LOG_WARNING("MessageTopicClassifier: Message trait class is not found.");
  }

  return messageIntent;
}

void MessageTopicClassifier::buildTraitTemplate(ScTemplate & traitTemplate, ScAddr const & possibleMessageCLass)
{
  traitTemplate.TripleWithRelation(
      possibleMessageCLass,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> MessageClassificationAliasConstants::TRAIT_INCLUDED_CLASS_LINK_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      MessageClassificationKeynodes::nrel_wit_ai_idtf);
  traitTemplate.TripleWithRelation(
      ScType::NodeVarClass >> MessageClassificationAliasConstants::SET_OF_TRAITS_CLASS_ALIAS,
      ScType::EdgeDCommonVar,
      possibleMessageCLass,
      ScType::EdgeAccessVarPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_inclusion);
  traitTemplate.TripleWithRelation(
      MessageClassificationAliasConstants::SET_OF_TRAITS_CLASS_ALIAS,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> MessageClassificationAliasConstants::SET_OF_TRAITS_CLASS_LINK_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      MessageClassificationKeynodes::nrel_wit_ai_idtf);
}

ScAddrVector MessageTopicClassifier::processTraits(
    ScIterator3Ptr & possibleTraitIterator,
    json const & messageTrait,
    ScAddrVector & messageTraitClassElements,
    ScAddr const & messageAddr)
{
  std::string traitWitIdtf;
  std::string setOfTraitsWitIdtf;
  std::string traitClassIdtf;

  ScTemplate traitTemplate;
  ScAddr possibleMessageCLass;

  while (possibleTraitIterator->Next())
  {
    possibleMessageCLass = possibleTraitIterator->Get(2);
    buildTraitTemplate(traitTemplate, possibleMessageCLass);

    ScTemplateSearchResult traitTemplateResult;
    context->HelperSearchTemplate(traitTemplate, traitTemplateResult);
    traitTemplate.Clear();

    if (traitTemplateResult.Size() == 1)
    {
      ScAddr setOfTraitsLink =
          traitTemplateResult[0][MessageClassificationAliasConstants::SET_OF_TRAITS_CLASS_LINK_ALIAS];
      ScAddr traitLink = traitTemplateResult[0][MessageClassificationAliasConstants::TRAIT_INCLUDED_CLASS_LINK_ALIAS];

      traitWitIdtf = utils::CommonUtils::getLinkContent(context, traitLink);
      setOfTraitsWitIdtf = utils::CommonUtils::getLinkContent(context, setOfTraitsLink);
      traitClassIdtf = messageTrait.at(setOfTraitsWitIdtf).at(0).at(WitAiConstants::VALUE);

      if (traitClassIdtf == traitWitIdtf)
      {
        SC_LOG_DEBUG(
            "MessageTopicClassifier: found " + context->HelperGetSystemIdtf(possibleMessageCLass) + " trait class");
        ScAddr messageTraitClassEdge =
            context->CreateEdge(ScType::EdgeAccessConstPosPerm, possibleMessageCLass, messageAddr);
        messageTraitClassElements.push_back(possibleMessageCLass);
        messageTraitClassElements.push_back(messageTraitClassEdge);

        return messageTraitClassElements;
      }
    }
  }
  return messageTraitClassElements;
}

ScAddrVector MessageTopicClassifier::getMessageEntity(ScAddr const & messageAddr, json const & witResponse)
{
  ScAddrVector messageEntitiesElements;

  json const messageEntity = getMessageEntities(witResponse);
  if (!messageEntity.empty())
  {
    ScIterator3Ptr possibleEntityIterator = context->Iterator3(
        MessageClassificationKeynodes::concept_entity_possible_class,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConstClass);

    messageEntitiesElements =
        processEntities(possibleEntityIterator, messageEntity, messageEntitiesElements, messageAddr);
  }

  return messageEntitiesElements;
}

json MessageTopicClassifier::getMessageEntities(json const & witResponse)
{
  json messageEntity;
  try
  {
    messageEntity = witResponse.at(WitAiConstants::ENTITIES);
  }
  catch (...)
  {
    SC_LOG_WARNING("MessageTopicClassifier: Message entities are not found.");
  }

  return messageEntity;
}

void MessageTopicClassifier::buildEntityTemplate(ScTemplate & entityTemplate, ScAddr const & possibleEntityClass)
{
  entityTemplate.TripleWithRelation(
      possibleEntityClass,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> MessageClassificationAliasConstants::ENTITY_CLASS_LINK_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      MessageClassificationKeynodes::nrel_wit_ai_idtf);
  entityTemplate.TripleWithRelation(
      possibleEntityClass,
      ScType::EdgeDCommonVar,
      ScType::NodeVar >> MessageClassificationAliasConstants::ENTITY_SET_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      MessageClassificationKeynodes::nrel_entity_possible_role);
  entityTemplate.Triple(  // TODO: check formalisation. Why there is a set?
      MessageClassificationAliasConstants::ENTITY_SET_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVarRole >> MessageClassificationAliasConstants::ENTITY_ROLE_ALIAS);
  entityTemplate.TripleWithRelation(
      MessageClassificationAliasConstants::ENTITY_ROLE_ALIAS,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> MessageClassificationAliasConstants::ENTITY_ROLE_LINK_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      MessageClassificationKeynodes::nrel_wit_ai_idtf);
}

ScAddrVector MessageTopicClassifier::processEntities(
    ScIterator3Ptr & possibleEntityIterator,
    json const & messageEntity,
    ScAddrVector & messageEntitiesElements,
    ScAddr const & messageAddr)
{
  std::string entityIdtf;
  std::map<std::string, std::string> entityIdtfToRole;

  std::string entityRoleIdtf;
  for (auto const & [key, value] : messageEntity.items())
  {
    entityRoleIdtf = key.substr(key.find(':') + 1);
    for (auto const & valueItem : value)
    {
      entityIdtf = valueItem.at(WitAiConstants::BODY);
      entityIdtfToRole.insert({entityIdtf, entityRoleIdtf});
    }
  }

  ScTemplate entityTemplate;
  ScAddr possibleEntityClass;

  while (possibleEntityIterator->Next())
  {
    possibleEntityClass = possibleEntityIterator->Get(2);
    buildEntityTemplate(entityTemplate, possibleEntityClass);

    ScTemplateSearchResult entityTemplateResult;
    context->HelperSearchTemplate(entityTemplate, entityTemplateResult);
    entityTemplate.Clear();

    if (entityTemplateResult.Size() == 1)
    {
      ScAddr const & entityRole = entityTemplateResult[0][MessageClassificationAliasConstants::ENTITY_ROLE_ALIAS];
      ScAddr const & entityLink = entityTemplateResult[0][MessageClassificationAliasConstants::ENTITY_CLASS_LINK_ALIAS];
      ScAddr const & entityRoleLink =
          entityTemplateResult[0][MessageClassificationAliasConstants::ENTITY_ROLE_LINK_ALIAS];

      std::string entityWitAiIdtf = utils::CommonUtils::getLinkContent(context, entityLink);
      std::string const & entityRoleWitAiIdtf = utils::CommonUtils::getLinkContent(context, entityRoleLink);
      std::string entitiesKey = entityWitAiIdtf.append(":").append(entityRoleWitAiIdtf);

      ScIterator3Ptr const entityClassIterator =
          context->Iterator3(possibleEntityClass, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
      ScAddr entityAddr;
      while (entityClassIterator->Next())
      {
        entityAddr = entityClassIterator->Get(2);
        std::string const & idtf =
            utils::CommonUtils::getMainIdtf(context, entityAddr, {scAgentsCommon::CoreKeynodes::lang_ru});
        for (auto const & [entitySameIdtf, entitySameRoleIdtf] : entityIdtfToRole)
        {
          if (idtf == entitySameIdtf)
          {
            SC_LOG_DEBUG("MessageTopicClassifier: found " + context->HelperGetSystemIdtf(entityAddr) + " entity");
            ScAddr messageEntityEdge = context->CreateEdge(ScType::EdgeAccessConstPosPerm, messageAddr, entityAddr);
            ScAddr messageEntityRoleEdge =
                context->CreateEdge(ScType::EdgeAccessConstPosPerm, entityRole, messageEntityEdge);

            messageEntitiesElements.push_back(entityAddr);
            messageEntitiesElements.push_back(entityRole);
            messageEntitiesElements.push_back(messageEntityEdge);
            messageEntitiesElements.push_back(messageEntityRoleEdge);

            entityIdtfToRole.erase(entitySameIdtf);
          }
        }
      }
    }
  }

  for (auto const & [notFoundEntitiesIdtf, notFoundEntitiesRoles] : entityIdtfToRole)
  {
    ScAddr const & createdEntity = context->CreateLink();
    context->SetLinkContent(createdEntity, notFoundEntitiesIdtf);
    ScAddr const & createdEntityEdge = context->CreateEdge(ScType::EdgeAccessConstPosPerm, commonModule::Keynodes::lang_en, createdEntity);
    ScAddr const & messageEntityEdge = context->CreateEdge(ScType::EdgeAccessConstPosPerm, messageAddr, createdEntity);
    ScAddr const & entityRole = context->HelperResolveSystemIdtf(notFoundEntitiesRoles, ScType::NodeConstRole);
    ScAddr const & messageEntityRoleEdge = context->CreateEdge(ScType::EdgeAccessConstPosPerm, entityRole, messageEntityEdge);

    SC_LOG_DEBUG("MessageTopicClassifier: generated " << notFoundEntitiesIdtf << " entity");
    SC_LOG_DEBUG("MessageTopicClassifier: generated " << notFoundEntitiesRoles << " role");

    messageEntitiesElements.push_back(createdEntity);
    messageEntitiesElements.push_back(createdEntityEdge);
    messageEntitiesElements.push_back(messageEntityEdge);
    messageEntitiesElements.push_back(messageEntityRoleEdge);
    messageEntitiesElements.push_back(entityRole);
    messageEntitiesElements.push_back(commonModule::Keynodes::lang_en);
  }

  return messageEntitiesElements;
}

}  // namespace messageClassificationModule
