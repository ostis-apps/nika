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

  ScAddr const & firstStation = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::first);
  ScAddr const & secondStation = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::second);

  if(utils::CommonUtils::getLinkContent(&m_memoryCtx, firstStation) == utils::CommonUtils::getLinkContent(&m_memoryCtx, secondStation))
  {
    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Смешно -_-");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    SC_LOG_DEBUG("RegistrationAgent finished");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
    return SC_RESULT_OK;
  }

  ScAddrVector const & possibleFirstLinks = m_memoryCtx.FindLinksByContent(utils::CommonUtils::getLinkContent(&m_memoryCtx, firstStation));
    
  ScAddrVector const & possibleSecondLinks = m_memoryCtx.FindLinksByContent(utils::CommonUtils::getLinkContent(&m_memoryCtx, secondStation));

  auto busWosFound = false;
    
      for(ScAddr const & possibleFirstLink : possibleFirstLinks)
      {
        if(!busWosFound)
        {
          for(ScAddr const & possibleSecondLink : possibleSecondLinks)
          {
            if(!busWosFound)
            {
              ScTemplate getBus;
              getBus.Triple(
                TestKeynodes::concept_bus,
                ScType::EdgeAccessVarPosPerm,
                ScType::NodeConst >> "_bus"
              );
              getBus.TripleWithRelation(
                "_bus",
                ScType::EdgeDCommonVar,
                ScType::NodeConst >> "_busNumber",
                ScType::EdgeAccessVarPosPerm,
                TestKeynodes::nrel_number_of_route
              );
              getBus.TripleWithRelation(
                "_bus",
                ScType::EdgeDCommonVar,
                ScType::NodeConstStruct >> "_frame",
                ScType::EdgeAccessVarPosPerm,
                TestKeynodes::nrel_route
              );
              getBus.TripleWithRelation(
                "_bus",
                ScType::EdgeDCommonVar,
                ScType::NodeConstStruct >> "_timeTable",
                ScType::EdgeAccessVarPosPerm,
                TestKeynodes::nrel_time
              );
              getBus.Triple(
                "_frame",
                ScType::EdgeAccessVarPosPerm,
                ScType::NodeConst >> "_firstStation"
              );
              getBus.TripleWithRelation(
                "_firstStation",
                ScType::EdgeDCommonVar,
                possibleFirstLink,
                ScType::EdgeAccessVarPosPerm,
                TestKeynodes::nrel_main_idtf
              );
              getBus.Triple(
                "_frame",
                ScType::EdgeAccessVarPosPerm,
                ScType::NodeConst >> "_secondStation"
              );
              getBus.TripleWithRelation(
                "_secondStation",
                ScType::EdgeDCommonVar,
                possibleSecondLink,
                ScType::EdgeAccessVarPosPerm,
                TestKeynodes::nrel_main_idtf
              );
              ScTemplateSearchResult getUser;
              m_memoryCtx.HelperSearchTemplate(getBus, getUser);
              if(getUser.IsEmpty() == SC_FALSE)
              {
                busWosFound = true;
                std::string busNumber = utils::CommonUtils::getLinkContent(&m_memoryCtx, getUser[0]["_busNumber"]);
                std::string timeTable = utils::CommonUtils::getLinkContent(&m_memoryCtx, getUser[0]["_timeTable"]);

                messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Для вас был подобран " + busNumber + " автобус.<br><br>" + timeTable);

              }
            }
          }
        }
      }
    
    if (!busWosFound)
    {
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Извините, я не смог найти подходящий для вас транспорт(");
    }
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
  SC_LOG_DEBUG("RegistrationAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
  return SC_RESULT_OK;
}

bool RegistrationAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      TestKeynodes::action_registration, actionAddr, ScType::EdgeAccessConstPosPerm);
}

std::string RegistrationAgent::hashToString(const unsigned char* hash, size_t hashLength) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < hashLength; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }
    return ss.str();
}

std::string RegistrationAgent::encryptor(std::string password){
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
