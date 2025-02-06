#include "MessageReplyAgent.hpp"

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "generator/MessageHistoryGenerator.hpp"
#include "keynodes/MessageReplyKeynodes.hpp"

using namespace messageReplyModule;

ScResult MessageReplyAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr linkAddr = action.GetArgument(ScKeynodes::rrel_1);
  ScAddr chatAddr = action.GetArgument(ScKeynodes::rrel_2);
  ScAddr processingProgramAddr = getMessageProcessingProgram();
  ScAddr authorAddr = utils::IteratorUtils::getAnyByOutRelation(&m_context, action, MessageReplyKeynodes::nrel_authors);
  if (!processingProgramAddr.IsValid())
  {
    m_logger.Error("Message processing program not found.");
    return action.FinishUnsuccessfully();
  }
  if (!linkIsValid(linkAddr))
  {
    m_logger.Error("Message link not found.");
    return action.FinishUnsuccessfully();
  }
  if (!chatAddr.IsValid())
  {
    m_logger.Error("Message chat not found.");
    return action.FinishUnsuccessfully();
  }
  if (!authorAddr.IsValid())
  {
    m_logger.Error("Message author not found.");
    return action.FinishUnsuccessfully();
  }

  MessageHistoryGenerator generator = MessageHistoryGenerator(&m_context);

  ScAddr messageAddr;
  try
  {
    messageAddr = generateMessage(authorAddr, linkAddr);
    generator.addMessageToDialog(chatAddr, messageAddr);
  }
  catch (std::runtime_error & exception)
  {
    m_logger.Error(exception.what());
    return action.FinishUnsuccessfully();
  }

  ScAction actionToInterpret = m_context.GenerateAction(MessageReplyKeynodes::action_interpret_non_atomic_action);
  actionToInterpret.SetArguments(processingProgramAddr, generateNonAtomicActionArgsSet(messageAddr));

  if (!actionToInterpret.InitiateAndWait(WAIT_TIME) || !actionToInterpret.IsFinishedSuccessfully())
  {
    m_logger.Error("Action wait time expired or action not finished successfully");
    return action.FinishUnsuccessfully();
  }

  ScAddr answerAddr;
  try
  {
    answerAddr = generateAnswer(messageAddr);
    ScTemplate replySearchTemplate;
    replySearchTemplate.Quintuple(
        messageAddr,
        ScType::EdgeDCommonVar,
        ScType::NodeVar >> "_reply_message",
        ScType::EdgeAccessVarPosPerm,
        MessageReplyKeynodes::nrel_reply);
    ScTemplateSearchResult searchResult;
    m_context.SearchByTemplate(replySearchTemplate, searchResult);

    ScAddr const & replyMessageAddr = searchResult[0]["_reply_message"];
    utils::GenerationUtils::generateRelationBetween(
        &m_context, replyMessageAddr, MessageReplyKeynodes::myself, MessageReplyKeynodes::nrel_authors);

    generator.addMessageToDialog(chatAddr, replyMessageAddr);
  }
  catch (std::runtime_error & exception)
  {
    m_logger.Error(exception.what());
    return action.FinishUnsuccessfully();
  }

  action.SetResult(answerAddr);
  return action.FinishSuccessfully();
}

ScAddr MessageReplyAgent::GetActionClass() const
{
  return MessageReplyKeynodes::action_reply_to_message;
}

ScAddr MessageReplyAgent::getMessageProcessingProgram()
{
  return MessageReplyKeynodes::message_processing_program;
}

ScAddr MessageReplyAgent::generateMessage(ScAddr const & authorAddr, ScAddr const & linkAddr)
{
  static std::string const USER_MESSAGE_ALIAS = "_user_message";
  static std::string const TRANSLATION_NODE_ALIAS = "_translation_node";

  ScTemplate userMessageTemplate;
  userMessageTemplate.Triple(
      MessageReplyKeynodes::concept_message, ScType::EdgeAccessVarPosPerm, ScType::NodeVar >> USER_MESSAGE_ALIAS);
  userMessageTemplate.Quintuple(
      USER_MESSAGE_ALIAS,
      ScType::EdgeDCommonVar,
      authorAddr,
      ScType::EdgeAccessVarPosPerm,
      MessageReplyKeynodes::nrel_authors);
  userMessageTemplate.Quintuple(
      ScType::NodeVar >> TRANSLATION_NODE_ALIAS,
      ScType::EdgeDCommonVar,
      USER_MESSAGE_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      MessageReplyKeynodes::nrel_sc_text_translation);
  userMessageTemplate.Triple(TRANSLATION_NODE_ALIAS, ScType::EdgeAccessVarPosPerm, linkAddr);
  ScTemplateGenResult templateGenResult;

  m_context.GenerateByTemplate(userMessageTemplate, templateGenResult);

  return templateGenResult[USER_MESSAGE_ALIAS];
}

ScAddr MessageReplyAgent::generateNonAtomicActionArgsSet(ScAddr const & messageAddr)
{
  static std::string const ARGS_SET_ALIAS = "_args_set";

  ScTemplate argsSetTemplate;
  argsSetTemplate.Quintuple(
      ScType::NodeVar >> ARGS_SET_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      messageAddr,
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::rrel_1);
  ScTemplateGenResult templateGenResult;

  m_context.GenerateByTemplate(argsSetTemplate, templateGenResult);

  return templateGenResult[ARGS_SET_ALIAS];
}

ScAddr MessageReplyAgent::generateAnswer(ScAddr const & messageAddr)
{
  static std::string const REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS = "_reply_message_relation_pair_arc";
  static std::string const REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS = "_reply_message_relation_access_arc";
  static std::string const REPLY_MESSAGE_ALIAS = "_reply_message";
  static std::string const ANSWER_ALIAS = "_answer";

  ScTemplate replySearchTemplate;
  replySearchTemplate.Quintuple(
      messageAddr,
      ScType::EdgeDCommonVar >> REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS,
      ScType::NodeVar >> REPLY_MESSAGE_ALIAS,
      ScType::EdgeAccessVarPosPerm >> REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS,
      MessageReplyKeynodes::nrel_reply);
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(replySearchTemplate, searchResult);
  if (searchResult.Size() != 1)
  {
    throw std::runtime_error("Reply message not generated.");
  }

  ScAddr answerNodeAddr = m_context.GenerateNode(ScType::NodeConstStruct);
  ScTemplate answerGenerationTemplate;
  answerGenerationTemplate.Triple(answerNodeAddr, ScType::EdgeAccessVarPosPerm, messageAddr);
  answerGenerationTemplate.Triple(
      answerNodeAddr, ScType::EdgeAccessVarPosPerm, searchResult[0][REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS]);
  answerGenerationTemplate.Triple(answerNodeAddr, ScType::EdgeAccessVarPosPerm, searchResult[0][REPLY_MESSAGE_ALIAS]);
  answerGenerationTemplate.Triple(
      answerNodeAddr, ScType::EdgeAccessVarPosPerm, searchResult[0][REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS]);
  answerGenerationTemplate.Triple(answerNodeAddr, ScType::EdgeAccessVarPosPerm, MessageReplyKeynodes::nrel_reply);

  ScAddrVector classes;
  ScAddrVector classesArcs;
  ScIterator3Ptr classesIt =
      m_context.CreateIterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, messageAddr);

  while (classesIt->Next())
  {
    classes.emplace_back(classesIt->Get(0));
    classesArcs.emplace_back(classesIt->Get(1));
  }

  for (size_t i = 0; i < classes.size(); i++)
  {
    answerGenerationTemplate.Triple(answerNodeAddr, ScType::EdgeAccessVarPosPerm, classes.at(i));
    answerGenerationTemplate.Triple(answerNodeAddr, ScType::EdgeAccessVarPosPerm, classesArcs.at(i));
  }

  ScTemplateGenResult templateGenResult;

  m_context.GenerateByTemplate(answerGenerationTemplate, templateGenResult);

  return answerNodeAddr;
}

bool MessageReplyAgent::linkIsValid(ScAddr const & linkAddr)
{
  if (utils::CommonUtils::checkType(&m_context, linkAddr, ScType::LinkConst))
  {
    if (!hasLanguage(linkAddr))
      return false;
    bool isSoundValid = soundLinkIsValid(linkAddr);
    bool isTextValid = textLinkIsValid(linkAddr);
    if (isSoundValid)
    {
      m_logger.Debug("Sound link is found");
      return true;
    }
    if (isTextValid)
    {
      m_logger.Debug("Text link is found");
      return true;
    }
  }
  return false;
}

bool MessageReplyAgent::hasLanguage(ScAddr const & linkAddr)
{
  static std::string const LINK_LANGUAGE_CLASS_ALIAS = "_link_language_class";

  ScTemplate languageTemplate;
  languageTemplate.Triple(
      MessageReplyKeynodes::languages, ScType::EdgeAccessVarPosPerm, ScType::NodeVarClass >> LINK_LANGUAGE_CLASS_ALIAS);
  languageTemplate.Triple(LINK_LANGUAGE_CLASS_ALIAS, ScType::EdgeAccessVarPosPerm, linkAddr);
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(languageTemplate, searchResult);
  return searchResult.Size() == 1;
}

bool MessageReplyAgent::soundLinkIsValid(ScAddr const & linkAddr)
{
  ScTemplate soundLinkTemplate;
  soundLinkTemplate.Triple(MessageReplyKeynodes::concept_sound_file, ScType::EdgeAccessVarPosPerm, linkAddr);
  soundLinkTemplate.Triple(MessageReplyKeynodes::format_wav, ScType::EdgeAccessVarPosPerm, linkAddr);
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(soundLinkTemplate, searchResult);
  return searchResult.Size() == 1;
}

bool MessageReplyAgent::textLinkIsValid(ScAddr const & linkAddr)
{
  ScTemplate textLinkTemplate;
  textLinkTemplate.Triple(MessageReplyKeynodes::concept_text_file, ScType::EdgeAccessVarPosPerm, linkAddr);
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(textLinkTemplate, searchResult);
  return searchResult.Size() == 1;
}
