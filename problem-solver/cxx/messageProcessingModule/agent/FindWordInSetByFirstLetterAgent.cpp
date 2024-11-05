#include "FindWordInSetByFirstLetterAgent.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/MessageKeynodes.hpp"
#include "keynodes/MessageProcessingKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace utils;

namespace messageProcessingModule
{

ScResult FindWordInSetByFirstLetterAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr const & messageAddr = action.GetArgument(ScKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    SC_AGENT_LOG_ERROR("The message isn’t valid");
    return action.FinishUnsuccessfully();
  }

  if (!m_context.CheckConnector(
          MessageProcessingKeynodes::concept_message_about_find_word_by_first_letter,
          messageAddr,
          ScType::ConstPermPosArc))
  {
    SC_AGENT_LOG_DEBUG("The message isn’t about letter search");
    return action.FinishUnsuccessfully();
  }

  ScAddrVector answerElements;
  try
  {
    ScIterator3Ptr const & agentAnswerLinkIterator = m_context.CreateIterator3(
        MessageProcessingKeynodes::word_starts_with_required_letter_answer_phrase,
        ScType::ConstPermPosArc,
        ScType::ConstNodeLink);
    if (agentAnswerLinkIterator->Next())
    {
      m_context.EraseElement(agentAnswerLinkIterator->Get(2));
    }

    messageSearcher = std::make_unique<dialogControlModule::MessageSearcher>(&m_context);
    std::string messageText = getMessageText(messageAddr);
    ScAddr const & entityAddr = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, messageAddr, dialogControlModule::MessageKeynodes::rrel_entity);

    ScIterator3Ptr const & entityNodesIterator =
        m_context.CreateIterator3(entityAddr, ScType::ConstPermPosArc, ScType::ConstNode);

    std::string firstLetter;
    std::string firstWordLetter;

    firstLetter = messageText.substr(messageText.size() - 3, 2);
    std::stringstream resultStream;
    std::string result;
    std::string word;

    if (entityNodesIterator->Next())
    {
      word = utils::CommonUtils::getMainIdtf(&m_context, entityNodesIterator->Get(2), {ScKeynodes::lang_ru});
      firstWordLetter = word.substr(0, 2);
      if (firstLetter == firstWordLetter)
      {
        resultStream << word;
        SC_AGENT_LOG_DEBUG("Found word " << word);
      }
    }
    while (entityNodesIterator->Next())
    {
      word = utils::CommonUtils::getMainIdtf(&m_context, entityNodesIterator->Get(2), {ScKeynodes::lang_ru});
      firstWordLetter = word.substr(0, 2);
      if (firstLetter == firstWordLetter)
      {
        if (resultStream.str().size() == 0)
          resultStream << word;
        else
          resultStream << ", " << word;
        SC_AGENT_LOG_DEBUG("Found word " << word);
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
    SC_AGENT_LOG_DEBUG("Reply message is generated");
  }
  catch (utils::ScException const & exception)
  {
    SC_AGENT_LOG_ERROR(exception.Description());
    ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);

    SC_AGENT_LOG_DEBUG("Finished with an error");
    return action.FinishUnsuccessfully();
  }

  ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);
  return action.FinishSuccessfully();
}

ScAddr FindWordInSetByFirstLetterAgent::GetActionClass() const
{
  return MessageProcessingKeynodes::action_find_word_in_set_by_first_letter;
}

ScAddrVector FindWordInSetByFirstLetterAgent::createAnswer(std::string const & linkContent) const
{
  ScAddr const & answerLink = m_context.GenerateLink(ScType::ConstNodeLink);
  m_context.SetLinkContent(answerLink, linkContent);
  ScAddr const & ConstPermPosArc = m_context.GenerateConnector(
      ScType::ConstPermPosArc,
      MessageProcessingKeynodes::word_starts_with_required_letter_answer_phrase,
      answerLink);
  return {
      answerLink, ConstPermPosArc, MessageProcessingKeynodes::word_starts_with_required_letter_answer_phrase};
}

std::string FindWordInSetByFirstLetterAgent::getMessageText(ScAddr const & messageAddr) const
{
  ScAddr const & messageLink = messageSearcher->getMessageLink(messageAddr);
  std::string linkContent;
  if (!messageLink.IsValid())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Message link is not found.");
  }
  m_context.GetLinkContent(messageLink, linkContent);
  return linkContent;
}

}  // namespace messageProcessingModule
