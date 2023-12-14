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

#include "AuthorizationAgent.hpp"

using namespace testModule;
using namespace scAgentsCommon;
using namespace commonModule;
using namespace dialogControlModule;

SC_AGENT_IMPLEMENTATION(AuthorizationAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("AuthorizationAgent started");

  
  SC_LOG_ERROR("GG");
  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  SC_LOG_ERROR("GG");
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("AuthorizationAgent finished");
     return SC_RESULT_ERROR;
  }
  SC_LOG_ERROR("GG");
  dialogControlModule::MessageConstructionsGenerator messageConstructionGenerator = MessageConstructionsGenerator(&m_memoryCtx);
  SC_LOG_ERROR("GG");
  
  ScAddr const & dialog = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);
  SC_LOG_ERROR("GG");
  ScTemplate findMessageTextLink;
  SC_LOG_ERROR("GG");
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
  SC_LOG_ERROR("GG");
  std::string messageText = utils::CommonUtils::getLinkContent(&m_memoryCtx, messageTextLink[0]["_message_text_link"]);
  SC_LOG_ERROR("GG");
  
  SC_LOG_ERROR("GG");
  if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_absence_of_authorized_user, dialog, ScType::EdgeAccessConstPosPerm) && messageText.find("Пройти авторизацию.") == 0)
  {
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    ScAddr const &loginLink = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::email);
    
    ScAddr const &passwordLink = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::password);
    
    std::string textLogin = utils::CommonUtils::getLinkContent(&m_memoryCtx, loginLink);
    
    std::string textPassword = utils::CommonUtils::getLinkContent(&m_memoryCtx, passwordLink);

    ScAddrVector const & possibleLoginLinks = m_memoryCtx.FindLinksByContent(textLogin);
    
    ScAddrVector const & possiblePasswordLinks = m_memoryCtx.FindLinksByContent(textPassword);
    
    
    
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);

    auto userWasFound = false;
    
      for(ScAddr const & possibleLoginLink : possibleLoginLinks)
      {
        if(!userWasFound)
        {
          SC_LOG_ERROR("for1");
          for(ScAddr const & possiblePasswordLink : possiblePasswordLinks)
          {
            if(!userWasFound)
            {
              SC_LOG_ERROR("for2");
              ScTemplate findUser;
              findUser.Triple(
                TestKeynodes::concept_users,
                ScType::EdgeAccessVarPosPerm,
                ScType::NodeVar >> "_user"
              );
              findUser.TripleWithRelation(
                "_user",
                ScType::EdgeDCommonVar,
                possibleLoginLink,
                ScType::EdgeAccessVarPosPerm,
                TestKeynodes::nrel_login
              );
              findUser.TripleWithRelation(
                "_user",
                ScType::EdgeDCommonVar,
                possiblePasswordLink,
                ScType::EdgeAccessVarPosPerm,
                TestKeynodes::nrel_password
              );
              ScTemplateSearchResult getUser;
              m_memoryCtx.HelperSearchTemplate(findUser, getUser);
              if(getUser.IsEmpty() == SC_FALSE)
              {
                userWasFound = true;
                SC_LOG_ERROR("if");
                m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_authorized_user, getUser[0]["_user"]);
                SC_LOG_ERROR("if");
                messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Авторизация успешна.");
                SC_LOG_ERROR("12456456");
                ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_absence_of_authorized_user, ScType::EdgeAccessConstPosPerm, dialog);
                SC_LOG_ERROR("if");
                if (it3->Next())
                  m_memoryCtx.EraseElement(it3->Get(1));
                
                SC_LOG_ERROR("if");
                m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_success_authorization_status, dialog);
                SC_LOG_ERROR("if");
              }
            }
          }
        }
      }
    
    if (!userWasFound)
    {
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Неверно введённый логин и/или пароль.");
    }
    
    SC_LOG_ERROR("if");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    SC_LOG_ERROR("if");
    ScIterator3Ptr it3_user = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    if (it3_user->Next())
    {
      SC_LOG_DEBUG("AuthorizationAgent finished");
      utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode,getAnswer(it3_user->Get(2)), true);
      return SC_RESULT_OK;
    }
  }
  else if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_success_authorization_status, dialog, ScType::EdgeAccessConstPosPerm) && messageText.find("Пройти авторизацию.") == 0)
  {
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    SC_LOG_ERROR("!!!");
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
    SC_LOG_ERROR("!!!");
    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "В системе уже авторизован пользователь! <br> Выйдите из системы, чтобы по новому авторизоваться.");
    SC_LOG_ERROR("!!!");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    ScIterator3Ptr it3_user = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    if (it3_user->Next())
      SC_LOG_DEBUG("AuthorizationAgent finished");
      utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode,replyAddr, true);
      return SC_RESULT_OK;
  }
  else if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_success_authorization_status, dialog, ScType::EdgeAccessConstPosPerm) && messageText == "Выйти из аккаунта")
  {
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    SC_LOG_ERROR("???");
    ScIterator3Ptr it3_exit = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    SC_LOG_ERROR("???");
    if (it3_exit->Next())
      m_memoryCtx.EraseElement(it3_exit->Get(1));
    SC_LOG_ERROR("???");
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_absence_of_authorized_user, dialog);
    SC_LOG_ERROR("???");
    ScIterator3Ptr it3_status = m_memoryCtx.Iterator3(TestKeynodes::concept_success_authorization_status, ScType::EdgeAccessConstPosPerm, dialog);
    SC_LOG_ERROR("???");
    if(it3_status->Next())
      m_memoryCtx.EraseElement(it3_status->Get(1));
    SC_LOG_ERROR("???");
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
    SC_LOG_ERROR("???");
    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Успешный выход из аккаунта.");
    SC_LOG_ERROR("???");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    SC_LOG_ERROR("???");
    ScIterator3Ptr it3_user = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    if (it3_user->Next())
      SC_LOG_DEBUG("AuthorizationAgent finished");
      utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode,replyAddr, true);
      return SC_RESULT_OK;
  }
  else if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_absence_of_authorized_user, dialog, ScType::EdgeAccessConstPosPerm) && messageText == "Выйти из аккаунта")
  {
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    SC_LOG_ERROR("...");
    
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);

    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Вы не авторизованы.");

    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    ScIterator3Ptr it3_user = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    if (it3_user->Next())
      SC_LOG_DEBUG("AuthorizationAgent finished");
      utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode,replyAddr, true);
      return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("AuthorizationAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
  return SC_RESULT_OK;

  
}

ScAddrVector AuthorizationAgent::getAnswer(ScAddr const & user)
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

bool AuthorizationAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      TestKeynodes::action_authorization, actionAddr, ScType::EdgeAccessConstPosPerm);
}
