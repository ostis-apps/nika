// подключение полезных утилит
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-memory/kpm/sc_agent.hpp"

#include "keynodes/Keynodes.hpp"

#include "LetterAgent.hpp"
#include <string>

using namespace scAgentsCommon;
using namespace utils;

namespace exampleModule
{
SC_AGENT_IMPLEMENTATION(LetterAgent)
{   
  messageSearcher = std::make_unique<dialogControlModule::MessageSearcher>(&m_memoryCtx);
  ScAddr actionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);
  ScAddr actionAddr = otherAddr;

  std::string firtsLetter;
  std::string firtsWordLetter;
  
  // обязательная часть в теле агента, служит фильтром
  // агентов по принадлежности к классу агентов  
  if (!checkActionClass(actionAddr))
  {
      return SC_RESULT_OK;
  }
  // логирование начала работы агента
  SC_LOG_DEBUG("LetterAgent started");

  ScAddrVector answerElements;

  ScAddr const & messageAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    SC_LOG_ERROR("LetterAgent: the message isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  if (!m_memoryCtx.HelperCheckEdge(
          Keynodes::concept_message_about_letter_search, messageAddr, ScType::EdgeAccessConstPosPerm))
  {
    SC_LOG_ERROR("LetterAgent: the message isn’t about letter search");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, ScAddrVector(), false);
    return SC_RESULT_ERROR;
  }

  try
  {
  std::string messageText = getMessageText(messageAddr);
  
  ScAddr const & entityAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, Keynodes::rrel_entity);
  
  //задание итератора для поиска элементов к котором есть связь с entity
  ScIterator3Ptr EntityNodesIterator = m_memoryCtx.Iterator3(
        entityAddr,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst);

  firtsLetter = messageText.substr(messageText.size()-3, 2);
  std::string result = "";

  while (EntityNodesIterator->Next())
  {
    ScAddr node = EntityNodesIterator->Get(2);
    std::string word =  utils::CommonUtils::getMainIdtf(&m_memoryCtx, node, {scAgentsCommon::CoreKeynodes::lang_ru});
    firtsWordLetter = word.substr(0,2); 
    if (firtsLetter == firtsWordLetter) {
        result += word + ' ';
    }
  }

  if (result == "") {
    result = "Таких слов нет";
  }
  


  
  const ScAddrVector & valueElements =
    addTempLinkWithRelation(messageAddr, result, scAgentsCommon::CoreKeynodes::nrel_answer);
  answerElements.insert(answerElements.cend(), valueElements.cbegin(), valueElements.cend());

  }
  catch (utils::ScException & exception)
  {
    SC_LOG_ERROR(exception.Description());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, false);
    SC_LOG_DEBUG("LetterAgent: finished with an error");
    return SC_RESULT_ERROR;
  }

    
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, true);
  SC_LOG_DEBUG("LetterAgent finished");
  return SC_RESULT_OK;
}

bool LetterAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(Keynodes::action_letter_search, actionAddr, ScType::EdgeAccessConstPosPerm);
}

ScAddrVector LetterAgent::addTempLinkWithRelation(ScAddr addr, const std::string & linkContent, ScAddr relation)
    const
{
  const ScAddr & link = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(link, ScStreamConverter::StreamFromString(linkContent));
  const ScAddr & edgeDCommon = m_memoryCtx.CreateEdge(ScType::EdgeDCommonConst, addr, link);
  const ScAddr & tempArc = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, relation, edgeDCommon);
  return {link, relation, edgeDCommon, tempArc};
}

std::string LetterAgent::getMessageText(ScAddr const & messageAddr) const
{
  
  ScAddr const messageLink = messageSearcher->getMessageLink(messageAddr);
  if (!messageLink.IsValid())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "LetterManager: message link is not found.");
  }
  return utils::CommonUtils::getLinkContent(&m_memoryCtx, messageLink);
}

}