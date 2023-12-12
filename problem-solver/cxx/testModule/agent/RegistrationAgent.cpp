#include <iostream>
#include <string>
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "keynodes/TestKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "keynodes/DialogKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "generator/MessageConstructionsGenerator.hpp"
#include "handler/LinkHandler.hpp"

#include "RegistrationAgent.hpp"

using namespace testModule;
using namespace scAgentsCommon;
using namespace commonModule;
using namespace dialogControlModule;

SC_AGENT_IMPLEMENTATION(RegistrationAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("RegistrationAgent started");

  
  
  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("RegistrationAgent finished");
     return SC_RESULT_ERROR;
  }
  
  dialogControlModule::MessageConstructionsGenerator messageConstructionGenerator = MessageConstructionsGenerator(&m_memoryCtx);
  auto LinkHandler = commonModule:: LinkHandler(&m_memoryCtx); 
  
  ScAddr const & dialog = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);
  
  ScTemplate findMessageTextLink;
  
  findMessageTextLink.TripleWithRelation(
    ScType::NodeVar >> "_tuple",
    ScType::EdgeDCommonVar,
    messageAddr,
    ScType::EdgeAccessVarPosPerm,
    DialogKeynodes::nrel_sc_text_translation
  );
  
  findMessageTextLink.Triple(
    "_tuple",
    ScType::EdgeAccessVarPosPerm,
    ScType::LinkVar >> "_message_text_link"
  );
  
  ScTemplateSearchResult messageTextLink;
  m_memoryCtx.HelperSearchTemplate(findMessageTextLink, messageTextLink);
  
  std::string messageText = utils::CommonUtils::getLinkContent(&m_memoryCtx, messageTextLink[0]["_message_text_link"]);
  
  if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_absence_of_authorized_user, dialog, ScType::EdgeAccessConstPosPerm) && messageText.find("Пройти регистрацию.") == 0)
  {
    ScAddr const &link_login = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::email);
    ScAddr const &link_password = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::password);
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);

    ScTemplate createTemplate;
    createTemplate.Triple(
      TestKeynodes::concept_users,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_user"
    );
    createTemplate.TripleWithRelation(
      "_user",
      ScType::EdgeDCommonVar,
      LinkHandler.createLink(utils::CommonUtils::getLinkContent(&m_memoryCtx, link_login)),
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::nrel_login
    );
    createTemplate.TripleWithRelation(
      "_user",
      ScType::EdgeDCommonVar,
      LinkHandler.createLink(utils::CommonUtils::getLinkContent(&m_memoryCtx, link_password)),
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::nrel_password
    );
    ScTemplateGenResult templateGenResult;
    m_memoryCtx.HelperGenTemplate(createTemplate, templateGenResult);

    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Регистрация успешна!");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);

    ScAddr const &user = getLinkConstructionLogin(utils::CommonUtils::getLinkContent(&m_memoryCtx, link_login));

    SC_LOG_DEBUG("RegistrationAgent finished");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode,getAnswer(user), true);
    return SC_RESULT_OK;
  }
  else if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_success_authorization_status, dialog, ScType::EdgeAccessConstPosPerm) && messageText.find("Пройти регистрацию.") == 0)
  {
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Чтобы пройти регистрацию, выйдите из аккаунта.");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    
    SC_LOG_DEBUG("RegistrationAgent finished");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("RegistrationAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
  return SC_RESULT_OK;
}

ScAddrVector RegistrationAgent::getAnswer(ScAddr const & user)
{
  ScTemplate answerTemplate;
  answerTemplate.Triple(
    TestKeynodes::concept_authorized_user,
    ScType::EdgeAccessVarPosPerm >> "_edge1",
    user
  );
  answerTemplate.Triple(
    TestKeynodes::concept_users,
    ScType::EdgeAccessVarPosPerm >> "_edge2",
    user
  );
  answerTemplate.TripleWithRelation(
    user,
    ScType::EdgeDCommonVar >> "_edge3",
    ScType::LinkVar >> "_login",
    ScType::EdgeAccessVarPosPerm >> "_edge4",
    TestKeynodes::nrel_login
  );
  answerTemplate.TripleWithRelation(
    user,
    ScType::EdgeDCommonVar >> "_edge5",
    ScType::LinkVar >> "_password",
    ScType::EdgeAccessVarPosPerm >> "_edge6",
    TestKeynodes::nrel_password
  );
  ScTemplateSearchResult answer;
  m_memoryCtx.HelperSearchTemplate(answerTemplate, answer);
  if (answer.IsEmpty() == SC_FALSE)
    return {answer[0]["_edge1"],answer[0]["_edge2"],answer[0]["_edge3"],answer[0]["_edge4"],
    answer[0]["_edge5"],answer[0]["_edge6"],answer[0]["_login"],
    answer[0]["_password"],user,
    TestKeynodes::concept_authorized_user, TestKeynodes::concept_users, TestKeynodes::nrel_login, 
    TestKeynodes::nrel_password};
  return {};                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
}

ScAddr RegistrationAgent::getLinkConstructionLogin(std::string const & text)
    {
      ScAddrVector const & possibleLinks = m_memoryCtx.FindLinksByContent(text); 
      for (ScAddr const & possibleLink : possibleLinks)
      {
        ScTemplate scTemplate;
        scTemplate.Triple(
          TestKeynodes::concept_users,
          ScType::EdgeAccessVarPosPerm,
          ScType::NodeVar >> "_user"
        );
        scTemplate.TripleWithRelation(
          "_user",
          ScType::EdgeDCommonVar,
          possibleLink,
          ScType::EdgeAccessVarPosPerm,
          TestKeynodes::nrel_login
        );
        ScTemplateSearchResult searchResult;
        m_memoryCtx.HelperSearchTemplate(scTemplate, searchResult);
        if (searchResult.IsEmpty() == SC_FALSE)
          return searchResult[0]["_user"];
      }
      return ScAddr::Empty;
    }

bool RegistrationAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      TestKeynodes::action_registration, actionAddr, ScType::EdgeAccessConstPosPerm);
}
