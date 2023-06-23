// Подключение полезных утилит из sc-machine
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "MessageKeynodes.generated.hpp"

#include "keynodes/MessageProcessingKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"

#include "FindWordInSetByFirstLetterAgent.hpp"

using namespace scAgentsCommon;
using namespace utils;

namespace messageProcessingModule
{
SC_AGENT_IMPLEMENTATION(FindWordInSetByFirstLetterAgent)
{
  // Проверка условия инициирования sc-агента
  ScAddr const & actionAddr = otherAddr;
  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }

  // Вывод сообщения о начале работы агента
  SC_LOG_DEBUG("FindWordInSetByFirstLetterAgent started");

  ScAddr const & messageAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    SC_LOG_ERROR("FindWordInSetByFirstLetterAgent: the message isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  if (!m_memoryCtx.HelperCheckEdge(
          MessageProcessingKeynodes::concept_message_about_find_word_by_first_letter,
          messageAddr,
          ScType::EdgeAccessConstPosPerm))
  {
    SC_LOG_ERROR("FindWordInSetByFirstLetterAgent: the message isn’t about letter search");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  ScAddrVector answerElements;
  try
  {
    ScIterator3Ptr const & agentAnswerLinkIterator = m_memoryCtx.Iterator3(
        MessageProcessingKeynodes::word_starts_with_required_letter_answer_phrase,
        ScType::EdgeAccessConstPosPerm,
        ScType::LinkConst);
    if (agentAnswerLinkIterator->Next())
    {
      m_memoryCtx.EraseElement(agentAnswerLinkIterator->Get(2));
    }

    messageSearcher = std::make_unique<dialogControlModule::MessageSearcher>(&m_memoryCtx);
    std::string messageText = getMessageText(messageAddr);

    ScAddr const & entityAddr = utils::IteratorUtils::getAnyByOutRelation(
        &m_memoryCtx, messageAddr, dialogControlModule::MessageKeynodes::rrel_entity);

    // Создание итератора для поиска элементов к котором есть связь с entity
    ScIterator3Ptr const & entityNodesIterator =
        m_memoryCtx.Iterator3(entityAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

    std::string firstLetter;
    std::string firstWordLetter;

    firstLetter = messageText.substr(messageText.size() - 3, 2);
    std::stringstream resultStream;
    std::string result;
    std::string word;

    if (entityNodesIterator->Next())
    {
      word = utils::CommonUtils::getMainIdtf(
          &m_memoryCtx, entityNodesIterator->Get(2), {scAgentsCommon::CoreKeynodes::lang_ru});
      firstWordLetter = word.substr(0, 2);
      if (firstLetter == firstWordLetter)
      {
        resultStream << word + ' ';
        SC_LOG_DEBUG("FindWordInSetByFirstLetterAgent: found word " << word);
      }
    }
    while (entityNodesIterator->Next())
    {
      word = utils::CommonUtils::getMainIdtf(
          &m_memoryCtx, entityNodesIterator->Get(2), {scAgentsCommon::CoreKeynodes::lang_ru});
      firstWordLetter = word.substr(0, 2);
      if (firstLetter == firstWordLetter)
      {
        resultStream << ' ' << word;
        SC_LOG_DEBUG("FindWordInSetByFirstLetterAgent: found word " << word);
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
    answerElements = createAnswer(result);
    SC_LOG_DEBUG("FindWordInSetByFirstLetterAgent: reply message is generated");
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(exception.Description());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, false);
    SC_LOG_DEBUG("FindWordInSetByFirstLetterAgent: finished with an error");
    return SC_RESULT_ERROR;
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, true);

  SC_LOG_DEBUG("FindWordInSetByFirstLetterAgent finished");
  return SC_RESULT_OK;
}

bool FindWordInSetByFirstLetterAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      MessageProcessingKeynodes::action_find_word_in_set_by_first_letter, actionAddr, ScType::EdgeAccessConstPosPerm);
}

ScAddrVector FindWordInSetByFirstLetterAgent::createAnswer(std::string const & linkContent) const
{
  ScAddr const & answerLink = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(answerLink, linkContent);
  ScAddr const & edgeAccessConstPosPerm = m_memoryCtx.CreateEdge(
      ScType::EdgeAccessConstPosPerm,
      MessageProcessingKeynodes::word_starts_with_required_letter_answer_phrase,
      answerLink);
  return {
      answerLink, edgeAccessConstPosPerm, MessageProcessingKeynodes::word_starts_with_required_letter_answer_phrase};
}

std::string FindWordInSetByFirstLetterAgent::getMessageText(ScAddr const & messageAddr) const
{
  ScAddr const & messageLink = messageSearcher->getMessageLink(messageAddr);
  if (!messageLink.IsValid())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "FindWordInSetByFirstLetterAgent: message link is not found.");
  }
  return utils::CommonUtils::getLinkContent(&m_memoryCtx, messageLink);
}

}  // namespace messageProcessingModule
