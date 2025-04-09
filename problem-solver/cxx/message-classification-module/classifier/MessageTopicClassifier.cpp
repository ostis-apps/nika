#include "MessageTopicClassifier.hpp"

#include "constants/MessageClassificationConstants.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include <common/keynodes/Keynodes.hpp>
#include <common/searcher/MessageSearcher.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace commonModule;

namespace messageClassificationModule
{
MessageTopicClassifier::MessageTopicClassifier(
    ScAgentContext * context,
    std::shared_ptr<WitAiClientInterface> const & client)
  : context(context)
  , client(client)
{
  messageSearcher = std::make_unique<MessageSearcher>(this->context);
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
  std::string linkContent;
  ScAddr const messageLink = messageSearcher->getMessageLink(messageAddr);
  if (!messageLink.IsValid())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Message link is not found.");
  }
  context->GetLinkContent(messageLink, linkContent);
  return linkContent;
}

ScAddrVector MessageTopicClassifier::getMessageIntentClass(ScAddr const & messageAddr, json const & witResponse)
{
  ScAddrVector messageIntentCLassElements;

  std::string const messageIntent = getMessageIntent(witResponse);
  if (messageIntent.empty())
  {
    ScAddr const & messageIntentCLassEdge = context->GenerateConnector(
        ScType::ConstPermPosArc, MessageClassificationKeynodes::concept_not_classified_by_intent_message, messageAddr);
    messageIntentCLassElements.push_back(MessageClassificationKeynodes::concept_not_classified_by_intent_message);
    messageIntentCLassElements.push_back(messageIntentCLassEdge);
    return messageIntentCLassElements;
  }

  ScIterator3Ptr const possibleIntentIterator = context->CreateIterator3(
      MessageClassificationKeynodes::concept_intent_possible_class, ScType::ConstPermPosArc, ScType::ConstNodeClass);

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
        SC_LOG_DEBUG("Found " << context->GetElementSystemIdentifier(possibleMessageCLass) << " intent class");
        ScAddr messageIntentCLassEdge =
            context->GenerateConnector(ScType::ConstPermPosArc, possibleMessageCLass, messageAddr);
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
    SC_LOG_WARNING("Message intent class is not found.");
  }

  return messageIntent;
}

std::vector<std::string> MessageTopicClassifier::getWitAiIdtfs(ScAddr const & messageClass)
{
  std::string linkContent;
  ScAddrVector witAiIdtfAddrs =
      utils::IteratorUtils::getAllByOutRelation(context, messageClass, MessageClassificationKeynodes::nrel_wit_ai_idtf);
  std::vector<std::string> witAiIdtfs;
  for (ScAddr const & witAiIdtfAddr : witAiIdtfAddrs)
  {
    context->GetLinkContent(witAiIdtfAddr, linkContent);
    witAiIdtfs.push_back(linkContent);
  }

  return witAiIdtfs;
}

ScAddrVector MessageTopicClassifier::getMessageTraitClass(ScAddr const & messageAddr, json const & witResponse)
{
  ScAddrVector messageTraitClassElements;

  json const messageTrait = getMessageTrait(witResponse);
  if (messageTrait.empty())
  {
    ScAddr const & messageIntentCLassEdge = context->GenerateConnector(
        ScType::ConstPermPosArc, MessageClassificationKeynodes::concept_not_classified_by_trait_message, messageAddr);
    messageTraitClassElements.push_back(MessageClassificationKeynodes::concept_not_classified_by_trait_message);
    messageTraitClassElements.push_back(messageIntentCLassEdge);
    return messageTraitClassElements;
  }

  ScIterator3Ptr possibleTraitIterator = context->CreateIterator3(
      MessageClassificationKeynodes::concept_trait_possible_class, ScType::ConstPermPosArc, ScType::ConstNodeClass);

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
    SC_LOG_WARNING("Message trait class is not found.");
  }

  return messageIntent;
}

void MessageTopicClassifier::buildTraitTemplate(ScTemplate & traitTemplate, ScAddr const & possibleMessageCLass)
{
  traitTemplate.Quintuple(
      possibleMessageCLass,
      ScType::VarCommonArc,
      ScType::VarNodeLink >> MessageClassificationAliasConstants::TRAIT_INCLUDED_CLASS_LINK_ALIAS,
      ScType::VarPermPosArc,
      MessageClassificationKeynodes::nrel_wit_ai_idtf);
  traitTemplate.Quintuple(
      ScType::VarNodeClass >> MessageClassificationAliasConstants::SET_OF_TRAITS_CLASS_ALIAS,
      ScType::VarCommonArc,
      possibleMessageCLass,
      ScType::VarPermPosArc,
      ScKeynodes::nrel_inclusion);
  traitTemplate.Quintuple(
      MessageClassificationAliasConstants::SET_OF_TRAITS_CLASS_ALIAS,
      ScType::VarCommonArc,
      ScType::VarNodeLink >> MessageClassificationAliasConstants::SET_OF_TRAITS_CLASS_LINK_ALIAS,
      ScType::VarPermPosArc,
      MessageClassificationKeynodes::nrel_wit_ai_idtf);
}

ScAddrVector MessageTopicClassifier::processTraits(
    ScIterator3Ptr & possibleTraitIterator,
    json const & messageTrait,
    ScAddrVector & messageTraitClassElements,
    ScAddr const & messageAddr)
{
  while (possibleTraitIterator->Next())
  {
    ScAddr possibleMessageCLass = possibleTraitIterator->Get(2);

    ScTemplate traitTemplate;
    buildTraitTemplate(traitTemplate, possibleMessageCLass);

    ScTemplateSearchResult traitTemplateResult;
    context->SearchByTemplate(traitTemplate, traitTemplateResult);
    traitTemplate.Clear();

    if (traitTemplateResult.Size() == 1)
    {
      std::string linkContent;
      ScAddr setOfTraitsLink =
          traitTemplateResult[0][MessageClassificationAliasConstants::SET_OF_TRAITS_CLASS_LINK_ALIAS];
      ScAddr traitLink = traitTemplateResult[0][MessageClassificationAliasConstants::TRAIT_INCLUDED_CLASS_LINK_ALIAS];

      std::string traitWitIdtf;
      context->GetLinkContent(traitLink, traitWitIdtf);
      std::string setOfTraitsWitIdtf;
      context->GetLinkContent(setOfTraitsLink, setOfTraitsWitIdtf);
      std::string traitClassIdtf = messageTrait.at(setOfTraitsWitIdtf).at(0).at(WitAiConstants::VALUE);

      if (traitClassIdtf == traitWitIdtf)
      {
        SC_LOG_DEBUG("Found " << context->GetElementSystemIdentifier(possibleMessageCLass) << " trait class");
        ScAddr messageTraitClassEdge =
            context->GenerateConnector(ScType::ConstPermPosArc, possibleMessageCLass, messageAddr);
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
    ScIterator3Ptr possibleEntityIterator = context->CreateIterator3(
        MessageClassificationKeynodes::concept_entity_possible_class, ScType::ConstPermPosArc, ScType::ConstNodeClass);

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
    SC_LOG_WARNING("Message entities are not found.");
  }

  return messageEntity;
}

void MessageTopicClassifier::buildEntityTemplate(ScTemplate & entityTemplate, ScAddr const & possibleEntityClass)
{
  entityTemplate.Quintuple(
      possibleEntityClass,
      ScType::VarCommonArc,
      ScType::VarNodeLink >> MessageClassificationAliasConstants::ENTITY_CLASS_LINK_ALIAS,
      ScType::VarPermPosArc,
      MessageClassificationKeynodes::nrel_wit_ai_idtf);
  entityTemplate.Quintuple(
      possibleEntityClass,
      ScType::VarCommonArc,
      ScType::VarNode >> MessageClassificationAliasConstants::ENTITY_SET_ALIAS,
      ScType::VarPermPosArc,
      MessageClassificationKeynodes::nrel_entity_possible_role);
  entityTemplate.Triple(  // TODO: check formalisation. Why there is a set?
      MessageClassificationAliasConstants::ENTITY_SET_ALIAS,
      ScType::VarPermPosArc,
      ScType::VarNodeRole >> MessageClassificationAliasConstants::ENTITY_ROLE_ALIAS);
  entityTemplate.Quintuple(
      MessageClassificationAliasConstants::ENTITY_ROLE_ALIAS,
      ScType::VarCommonArc,
      ScType::VarNodeLink >> MessageClassificationAliasConstants::ENTITY_ROLE_LINK_ALIAS,
      ScType::VarPermPosArc,
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
      entityIdtf = valueItem.at(WitAiConstants::VALUE);
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
    context->SearchByTemplate(entityTemplate, entityTemplateResult);
    entityTemplate.Clear();

    if (entityTemplateResult.Size() == 1)
    {
      ScAddr const & entityRole = entityTemplateResult[0][MessageClassificationAliasConstants::ENTITY_ROLE_ALIAS];
      ScAddr const & entityLink = entityTemplateResult[0][MessageClassificationAliasConstants::ENTITY_CLASS_LINK_ALIAS];
      ScAddr const & entityRoleLink =
          entityTemplateResult[0][MessageClassificationAliasConstants::ENTITY_ROLE_LINK_ALIAS];

      std::string entityWitAiIdtf;
      context->GetLinkContent(entityLink, entityWitAiIdtf);
      std::string entityRoleWitAiIdtf;
      context->GetLinkContent(entityRoleLink, entityRoleWitAiIdtf);
      std::string entitiesKey = entityWitAiIdtf.append(":").append(entityRoleWitAiIdtf);

      ScIterator3Ptr const entityClassIterator =
          context->CreateIterator3(possibleEntityClass, ScType::ConstPermPosArc, ScType::ConstNode);
      ScAddr entityAddr;
      while (entityClassIterator->Next())
      {
        entityAddr = entityClassIterator->Get(2);
        std::vector<std::string> identifiers;
        for (ScAddr const & relationToFindEntity : relationsToFindEntity)
        {
          identifiers.push_back(
              utils::CommonUtils::getIdtf(context, entityAddr, relationToFindEntity, {ScKeynodes::lang_ru}));
        }

        std::set<std::string> foundEntities;
        for (auto const & [entitySameIdtf, entitySameRoleIdtf] : entityIdtfToRole)
        {
          if (std::find(identifiers.begin(), identifiers.end(), entitySameIdtf) != identifiers.end())
          {
            SC_LOG_DEBUG("Found " << context->GetElementSystemIdentifier(entityAddr) << " entity");
            ScAddr messageEntityEdge = context->GenerateConnector(ScType::ConstPermPosArc, messageAddr, entityAddr);
            ScAddr messageEntityRoleEdge =
                context->GenerateConnector(ScType::ConstPermPosArc, entityRole, messageEntityEdge);

            messageEntitiesElements.push_back(entityAddr);
            messageEntitiesElements.push_back(entityRole);
            messageEntitiesElements.push_back(messageEntityEdge);
            messageEntitiesElements.push_back(messageEntityRoleEdge);

            foundEntities.insert(entitySameIdtf);
          }
        }

        for (auto const & entityIdtf : foundEntities)
        {
          if (entityIdtfToRole.find(entityIdtf) != entityIdtfToRole.cend())
            entityIdtfToRole.erase(entityIdtf);
        }
      }
    }
  }

  for (auto const & [notFoundEntitiesIdtf, notFoundEntitiesRoles] : entityIdtfToRole)
  {
    ScAddr const & createdEntity = context->GenerateLink();
    context->SetLinkContent(createdEntity, notFoundEntitiesIdtf);
    ScAddr const & createdEntityEdge =
        context->GenerateConnector(ScType::ConstPermPosArc, commonModule::Keynodes::lang_en, createdEntity);
    ScAddr const & messageEntityEdge = context->GenerateConnector(ScType::ConstPermPosArc, messageAddr, createdEntity);
    ScAddr const & entityRole = context->ResolveElementSystemIdentifier(notFoundEntitiesRoles, ScType::ConstNodeRole);
    ScAddr const & messageEntityRoleEdge =
        context->GenerateConnector(ScType::ConstPermPosArc, entityRole, messageEntityEdge);

    SC_LOG_DEBUG("Generated " << notFoundEntitiesIdtf << " entity");
    SC_LOG_DEBUG("Generated " << notFoundEntitiesRoles << " role");

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
