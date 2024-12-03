#include "RasaMessageTopicClassifier.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "constants/RasaMessageClassificationConstants.hpp"
#include "keynodes/Keynodes.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include "searcher/MessageSearcher.hpp"
#include "client/RasaClient.hpp"

namespace messageClassificationModule
{
RasaMessageTopicClassifier::RasaMessageTopicClassifier(
    ScAgentContext * context,
    std::shared_ptr<RasaClient> const & client)
  : context(context)
  , client(client)
{
  messageSearcher = std::make_unique<dialogControlModule::MessageSearcher>(this->context);
}

ScAddrVector RasaMessageTopicClassifier::classifyMessage(ScAddr const & messageAddr)
{
  ScAddrVector messageClassificationElements = {messageAddr};

  std::string const messageText = getMessageText(messageAddr);

  json const rasaResponse = client->getResponse(messageText);
  SC_LOG_INFO(rasaResponse);

  ScAddrVector const messageIntentElements = getMessageIntentClass(messageAddr, rasaResponse);
  messageClassificationElements.insert(
      messageClassificationElements.cend(), messageIntentElements.cbegin(), messageIntentElements.cend());

  ScAddrVector const messageEntitiesElements = getMessageEntity(messageAddr, rasaResponse);
  messageClassificationElements.insert(
      messageClassificationElements.cend(), messageEntitiesElements.cbegin(), messageEntitiesElements.cend());

  return messageClassificationElements; // ScAddr(intent), json(entities)
}

std::string RasaMessageTopicClassifier::getMessageText(ScAddr const & messageAddr)
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

ScAddrVector RasaMessageTopicClassifier::getMessageIntentClass(ScAddr const & messageAddr, json const & rasaResponse)
{
  ScAddrVector messageIntentCLassElements;

  std::string const messageIntent = getMessageIntent(rasaResponse);
  if (messageIntent.empty())
  {
    ScAddr const & messageIntentCLassEdge = context->GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        MessageClassificationKeynodes::concept_not_classified_by_intent_message,
        messageAddr);
    messageIntentCLassElements.push_back(MessageClassificationKeynodes::concept_not_classified_by_intent_message);
    messageIntentCLassElements.push_back(messageIntentCLassEdge);
    return messageIntentCLassElements;
  }


  ScIterator3Ptr const possibleIntentIterator = context->CreateIterator3(
      MessageClassificationKeynodes::concept_intent_possible_class,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConstClass);

  
  std::vector<std::string> rasaIdtfs;
  ScAddr possibleMessageCLass;
  while (possibleIntentIterator->Next())
  {
    possibleMessageCLass = possibleIntentIterator->Get(2);
    rasaIdtfs = getRasaIdtfs(possibleMessageCLass);

    for (std::string const & rasaIdtf : rasaIdtfs)
    {
      if (messageIntent == rasaIdtf)
      {
        SC_LOG_DEBUG("Found " << context->GetElementSystemIdentifier(possibleMessageCLass) << " intent class");
        ScAddr messageIntentCLassEdge =
            context->GenerateConnector(ScType::EdgeAccessConstPosPerm, possibleMessageCLass, messageAddr);
        messageIntentCLassElements.push_back(possibleMessageCLass);
        messageIntentCLassElements.push_back(messageIntentCLassEdge);
        return messageIntentCLassElements;
      }
    }
  }

  return messageIntentCLassElements;
}

std::string RasaMessageTopicClassifier::getMessageIntent(json const & rasaResponse)
{
  std::string messageIntent;
  try
  {
    messageIntent = rasaResponse.at(RasaConstants::INTENT).at(RasaConstants::NAME);
    SC_LOG_DEBUG("message intent: " << messageIntent);
  }
  catch (...)
  {
    SC_LOG_WARNING("Message intent class is not found.");
  }

  return messageIntent;
}

std::vector<std::string> RasaMessageTopicClassifier::getRasaIdtfs(ScAddr const & messageClass)
{
  std::string linkContent;
  ScAddrVector rasaIdtfAddrs =
      utils::IteratorUtils::getAllByOutRelation(context, messageClass, MessageClassificationKeynodes::nrel_rasa_idtf);
  std::vector<std::string> rasaIdtfs;
  for (ScAddr const & rasaIdtfAddr : rasaIdtfAddrs)
  {
    context->GetLinkContent(rasaIdtfAddr, linkContent);
    rasaIdtfs.push_back(linkContent);
  }

  return rasaIdtfs;
}

ScAddrVector RasaMessageTopicClassifier::getMessageEntity(ScAddr const & messageAddr, json const & rasaResponse)
{
  ScAddrVector messageEntitiesElements;

  std::vector<json> const messageEntity = getMessageEntities(rasaResponse);

  if (!messageEntity.empty())
  {
    ScIterator3Ptr possibleEntityIterator = context->CreateIterator3(
        MessageClassificationKeynodes::concept_entity_possible_class,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConstClass);

    messageEntitiesElements =
        processEntities(possibleEntityIterator, messageEntity, messageEntitiesElements, messageAddr);
  }

  return messageEntitiesElements;
}

std::vector<json> RasaMessageTopicClassifier::getMessageEntities(json const & rasaResponse)
{
  json messageEntity;
  try
  {
    messageEntity = rasaResponse.at(RasaConstants::ENTITIES);
  }
  catch (...)
  {
    SC_LOG_WARNING("Message entities are not found.");
  }

  return messageEntity;
}

void RasaMessageTopicClassifier::buildEntityTemplate(ScTemplate & entityTemplate, ScAddr const & possibleEntityClass)
{
  entityTemplate.Quintuple(
      possibleEntityClass,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> RasaMessageClassificationAliasConstants::ENTITY_CLASS_LINK_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      MessageClassificationKeynodes::nrel_rasa_idtf);
  entityTemplate.Quintuple(
      possibleEntityClass,
      ScType::EdgeDCommonVar,
      ScType::NodeVar >> RasaMessageClassificationAliasConstants::ENTITY_SET_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      MessageClassificationKeynodes::nrel_entity_possible_role);
  entityTemplate.Triple(  // TODO: check formalisation. Why there is a set?
      RasaMessageClassificationAliasConstants::ENTITY_SET_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVarRole >> RasaMessageClassificationAliasConstants::ENTITY_ROLE_ALIAS);
  entityTemplate.Quintuple(
      RasaMessageClassificationAliasConstants::ENTITY_ROLE_ALIAS,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> RasaMessageClassificationAliasConstants::ENTITY_ROLE_LINK_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      MessageClassificationKeynodes::nrel_rasa_idtf);
}

ScAddrVector RasaMessageTopicClassifier::processEntities(
    ScIterator3Ptr & possibleEntityIterator,
    std::vector<json> const & messageEntity,
    ScAddrVector & messageEntitiesElements,
    ScAddr const & messageAddr)
{
  std::string entityIdtf;
  std::map<std::string, std::string> entityIdtfToRole;

  std::string entityRoleIdtf;

  for (auto const & entityJson: messageEntity)
  { 
    for (auto const & [key, value] : entityJson.items())
    {
      if (key == RasaConstants::ENTITY_ROLE)
      {
        entityRoleIdtf = value;
      }
      else if (key == RasaConstants::VALUE)
      {
        entityIdtf = value;
      }
    }
    entityIdtfToRole.insert({entityIdtf, entityRoleIdtf});
    SC_LOG_DEBUG(entityIdtf << " " << entityRoleIdtf);
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
      ScAddr const & entityRole = entityTemplateResult[0][RasaMessageClassificationAliasConstants::ENTITY_ROLE_ALIAS];
      ScAddr const & entityLink = entityTemplateResult[0][RasaMessageClassificationAliasConstants::ENTITY_CLASS_LINK_ALIAS];
      ScAddr const & entityRoleLink =
          entityTemplateResult[0][RasaMessageClassificationAliasConstants::ENTITY_ROLE_LINK_ALIAS];

      std::string entityRasaIdtf;
      context->GetLinkContent(entityLink, entityRasaIdtf);
      std::string entityRoleRasaIdtf;
      context->GetLinkContent(entityRoleLink, entityRoleRasaIdtf);
      std::string entitiesKey = entityRasaIdtf.append(":").append(entityRoleRasaIdtf);

      ScIterator3Ptr const entityClassIterator =
          context->CreateIterator3(possibleEntityClass, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
      ScAddr entityAddr;
      while (entityClassIterator->Next())
      {
        entityAddr = entityClassIterator->Get(2);
        std::vector<std::string> identifiers;
        for (ScAddr const & relationToFindEntity : relationsToFindEntity)
        {
          auto idtf = utils::CommonUtils::getIdtf(context, entityAddr, relationToFindEntity, {ScKeynodes::lang_ru});

          identifiers.push_back(idtf);
        }

        for (auto const & [entitySameIdtf, entitySameRoleIdtf] : entityIdtfToRole)
        {
          if (std::find(identifiers.begin(), identifiers.end(), entitySameIdtf) != identifiers.end())
          {
            SC_LOG_DEBUG("Found " << context->GetElementSystemIdentifier(entityAddr) << " entity");
            ScAddr messageEntityEdge =
                context->GenerateConnector(ScType::EdgeAccessConstPosPerm, messageAddr, entityAddr);
            ScAddr messageEntityRoleEdge =
                context->GenerateConnector(ScType::EdgeAccessConstPosPerm, entityRole, messageEntityEdge);

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
    ScAddr const & createdEntity = context->GenerateLink();
    context->SetLinkContent(createdEntity, notFoundEntitiesIdtf);
    ScAddr const & createdEntityEdge =
        context->GenerateConnector(ScType::EdgeAccessConstPosPerm, commonModule::Keynodes::lang_en, createdEntity);
    ScAddr const & messageEntityEdge =
        context->GenerateConnector(ScType::EdgeAccessConstPosPerm, messageAddr, createdEntity);
    ScAddr const & entityRole = context->ResolveElementSystemIdentifier(notFoundEntitiesRoles, ScType::NodeConstRole);
    ScAddr const & messageEntityRoleEdge =
        context->GenerateConnector(ScType::EdgeAccessConstPosPerm, entityRole, messageEntityEdge);

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
