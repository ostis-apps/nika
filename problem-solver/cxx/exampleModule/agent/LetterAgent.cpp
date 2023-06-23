// Подключение полезных методов платформы
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "keynodes/LetterKeynodes.hpp"

#include "LetterAgent.hpp"

using namespace scAgentsCommon;
using namespace utils;

namespace exampleModule
{
SC_AGENT_IMPLEMENTATION(LetterAgent)
{
  // Проверка условия инициирования sc-агента
  ScAddr const & actionAddr = otherAddr;
  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }

  // Логирование начала работы агента
  SC_LOG_DEBUG("LetterAgent started");

  ScAddrVector answerElements;
  messageSearcher = std::make_unique<dialogControlModule::MessageSearcher>(&m_memoryCtx);

  std::string firstLetter;
  std::string firstWordLetter;

  ScAddr const & messageAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    SC_LOG_ERROR("LetterAgent: the message isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  if (!m_memoryCtx.HelperCheckEdge(
      LetterKeynodes::concept_message_about_letter_search, messageAddr, ScType::EdgeAccessConstPosPerm))
  {
    SC_LOG_ERROR("LetterAgent: the message isn’t about letter search");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, ScAddrVector(), false);
    return SC_RESULT_ERROR;
  }

  try
  {
  std::string messageText = getMessageText(messageAddr);
  
  ScAddr const & entityAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, LetterKeynodes::rrel_entity);
  
  // Создание итератора для поиска элементов к котором есть связь с entity
  ScIterator3Ptr const & entityNodesIterator = m_memoryCtx.Iterator3(
        entityAddr,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst);

  firstLetter = messageText.substr(messageText.size() - 3, 2);
  std::stringstream resultStream;
  std::string result;
  std::string word;

  if (entityNodesIterator->Next())
  {
    word = utils::CommonUtils::getMainIdtf(&m_memoryCtx, entityNodesIterator->Get(2), {scAgentsCommon::CoreKeynodes::lang_ru});
    firstWordLetter = word.substr(0, 2);
    if (firstLetter == firstWordLetter)
    {
      resultStream << word + ' ';
      SC_LOG_DEBUG("LetterAgent: found word " << word);
    }
  }
  while (entityNodesIterator->Next())
  {
    word = utils::CommonUtils::getMainIdtf(&m_memoryCtx, entityNodesIterator->Get(2), {scAgentsCommon::CoreKeynodes::lang_ru});
    firstWordLetter = word.substr(0, 2);
    if (firstLetter == firstWordLetter)
    {
      resultStream << ' ' << word;
      SC_LOG_DEBUG("LetterAgent: found word " << word);
    }
  }

  if (resultStream.str().empty())
  {
    result = "Таких слов нет";
  }
  else
  {
    result = resultStream.str();
  }

  answerElements = createAnswer(messageAddr, result, scAgentsCommon::CoreKeynodes::nrel_answer);
  }
  catch (utils::ScException const & exception)
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
  return m_memoryCtx.HelperCheckEdge(LetterKeynodes::action_letter_search, actionAddr, ScType::EdgeAccessConstPosPerm);
}

ScAddrVector LetterAgent::createAnswer(ScAddr const & messageAddr, std::string const & linkContent, ScAddr const & relation)
    const
{
  ScAddr const & answerLink = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(answerLink, linkContent);
  ScAddr const & edgeDCommon = m_memoryCtx.CreateEdge(ScType::EdgeDCommonConst, messageAddr, answerLink);
  ScAddr const & relationArc = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, relation, edgeDCommon);
  return {answerLink, relation, edgeDCommon, relationArc};
}

std::string LetterAgent::getMessageText(ScAddr const & messageAddr) const
{
  ScAddr const & messageLink = messageSearcher->getMessageLink(messageAddr);
  if (!messageLink.IsValid())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "LetterManager: message link is not found.");
  }
  return utils::CommonUtils::getLinkContent(&m_memoryCtx, messageLink);
}

}
