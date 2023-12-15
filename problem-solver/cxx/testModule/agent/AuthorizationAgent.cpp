#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/ssl.h>
#include <openssl/evp.h>
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

  
  
  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);

  if (!messageAddr.IsValid())
  {
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("AuthorizationAgent finished");
     return SC_RESULT_ERROR;
  }
  dialogControlModule::MessageConstructionsGenerator messageConstructionGenerator = MessageConstructionsGenerator(&m_memoryCtx);

  
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

  

  if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_absence_of_authorized_user, dialog, ScType::EdgeAccessConstPosPerm) && messageText.find("Пройти авторизацию.") == 0)
  {
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    ScAddr const &loginLink = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::email);
    
    ScAddr const &passwordLink = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::password);
    
    std::string textLogin = utils::CommonUtils::getLinkContent(&m_memoryCtx, loginLink);
    
    std::string textPassword = utils::CommonUtils::getLinkContent(&m_memoryCtx, passwordLink);
    std::string encryptedPasswordFromMessage = encryptor(textPassword);
    ScAddrVector const & possibleLoginLinks = m_memoryCtx.FindLinksByContent(textLogin);
    
    ScAddrVector const & possiblePasswordLinks = m_memoryCtx.FindLinksByContent(encryptedPasswordFromMessage);
    
    
    
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);

    auto userWasFound = false;
    
      for(ScAddr const & possibleLoginLink : possibleLoginLinks)
      {
        if(!userWasFound)
        {
          for(ScAddr const & possiblePasswordLink : possiblePasswordLinks)
          {
            if(!userWasFound)
            {
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

                m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_authorized_user, getUser[0]["_user"]);

                messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Авторизация успешна.");
       
                ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_absence_of_authorized_user, ScType::EdgeAccessConstPosPerm, dialog);
      
                if (it3->Next())
                  m_memoryCtx.EraseElement(it3->Get(1));

                m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_success_authorization_status, dialog);

              }
            }
          }
        }
      }
    
    if (!userWasFound)
    {
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Неверно введённый логин и/или пароль.");
    }
    
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);

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
  
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);

    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "В системе уже авторизован пользователь! <br> Выйдите из системы, чтобы по новому авторизоваться.");

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

    ScIterator3Ptr it3_exit = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

    if (it3_exit->Next())
      m_memoryCtx.EraseElement(it3_exit->Get(1));

    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_absence_of_authorized_user, dialog);

    ScIterator3Ptr it3_status = m_memoryCtx.Iterator3(TestKeynodes::concept_success_authorization_status, ScType::EdgeAccessConstPosPerm, dialog);

    if(it3_status->Next())
      m_memoryCtx.EraseElement(it3_status->Get(1));

    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);

    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Успешный выход из аккаунта.");

    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);

    ScIterator3Ptr it3_user = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    if (it3_user->Next())
      SC_LOG_DEBUG("AuthorizationAgent finished");
      utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode,replyAddr, true);
      return SC_RESULT_OK;
  }
  else if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_absence_of_authorized_user, dialog, ScType::EdgeAccessConstPosPerm) && messageText == "Выйти из аккаунта")
  {
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);

    
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

std::string AuthorizationAgent::hashToString(const unsigned char* hash, size_t hashLength) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < hashLength; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }
    return ss.str();
}

std::string AuthorizationAgent::encryptor(std::string password){
    const char *q = password.c_str();
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_DigestInit_ex(mdctx, md, NULL);

    EVP_DigestUpdate(mdctx, q, strlen(q));

    EVP_DigestFinal_ex(mdctx, hash, &hash_len);

    EVP_MD_CTX_free(mdctx);
    std::string result=hashToString(hash, hash_len);
    return result;
}
