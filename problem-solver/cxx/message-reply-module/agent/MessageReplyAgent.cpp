#include "MessageReplyAgent.hpp"

#include "generator/MessageHistoryGenerator.hpp"
#include "keynodes/MessageReplyKeynodes.hpp"
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace messageReplyModule;

MessageReplyAgent::MessageReplyAgent()
{
  m_logger = utils::ScLogger(utils::ScLogger::ScLogType::Console, "", utils::ScLogLevel::Debug);
}

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
    ScTemplate replySearchByTemplate;
    replySearchByTemplate.Quintuple(
        messageAddr,
        ScType::VarCommonArc,
        ScType::VarNode >> "_reply_message",
        ScType::VarPermPosArc,
        MessageReplyKeynodes::nrel_reply);
    ScTemplateSearchResult searchResult;
    m_context.SearchByTemplate(replySearchByTemplate, searchResult);

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
      MessageReplyKeynodes::concept_message, ScType::VarPermPosArc, ScType::VarNode >> USER_MESSAGE_ALIAS);
  userMessageTemplate.Quintuple(
      USER_MESSAGE_ALIAS, ScType::VarCommonArc, authorAddr, ScType::VarPermPosArc, MessageReplyKeynodes::nrel_authors);
  userMessageTemplate.Quintuple(
      ScType::VarNode >> TRANSLATION_NODE_ALIAS,
      ScType::VarCommonArc,
      USER_MESSAGE_ALIAS,
      ScType::VarPermPosArc,
      MessageReplyKeynodes::nrel_sc_text_translation);
  userMessageTemplate.Triple(TRANSLATION_NODE_ALIAS, ScType::VarPermPosArc, linkAddr);
  ScTemplateGenResult templateGenResult;

  m_context.GenerateByTemplate(userMessageTemplate, templateGenResult);

  return templateGenResult[USER_MESSAGE_ALIAS];
}

ScAddr MessageReplyAgent::generateNonAtomicActionArgsSet(ScAddr const & messageAddr)
{
  static std::string const ARGS_SET_ALIAS = "_args_set";

  ScTemplate argsSetTemplate;
  argsSetTemplate.Quintuple(
      ScType::VarNode >> ARGS_SET_ALIAS, ScType::VarPermPosArc, messageAddr, ScType::VarPermPosArc, ScKeynodes::rrel_1);
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

  ScTemplate replySearchByTemplate;
  replySearchByTemplate.Quintuple(
      messageAddr,
      ScType::VarCommonArc >> REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS,
      ScType::VarNode >> REPLY_MESSAGE_ALIAS,
      ScType::VarPermPosArc >> REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS,
      MessageReplyKeynodes::nrel_reply);
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(replySearchByTemplate, searchResult);
  if (searchResult.Size() != 1)
  {
    throw std::runtime_error("Reply message not generated.");
  }

  ScAddr answerNodeAddr = m_context.GenerateNode(ScType::ConstNodeStructure);
  ScTemplate answerGenerationTemplate;
  answerGenerationTemplate.Triple(answerNodeAddr, ScType::VarPermPosArc, messageAddr);
  answerGenerationTemplate.Triple(
      answerNodeAddr, ScType::VarPermPosArc, searchResult[0][REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS]);
  answerGenerationTemplate.Triple(answerNodeAddr, ScType::VarPermPosArc, searchResult[0][REPLY_MESSAGE_ALIAS]);
  answerGenerationTemplate.Triple(
      answerNodeAddr, ScType::VarPermPosArc, searchResult[0][REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS]);
  answerGenerationTemplate.Triple(answerNodeAddr, ScType::VarPermPosArc, MessageReplyKeynodes::nrel_reply);

  ScAddrVector classes;
  ScAddrVector classesArcs;
  ScIterator3Ptr classesIt = m_context.CreateIterator3(ScType::ConstNodeClass, ScType::ConstPermPosArc, messageAddr);

  while (classesIt->Next())
  {
    classes.emplace_back(classesIt->Get(0));
    classesArcs.emplace_back(classesIt->Get(1));
  }

  for (size_t i = 0; i < classes.size(); i++)
  {
    answerGenerationTemplate.Triple(answerNodeAddr, ScType::VarPermPosArc, classes.at(i));
    answerGenerationTemplate.Triple(answerNodeAddr, ScType::VarPermPosArc, classesArcs.at(i));
  }

  ScTemplateGenResult templateGenResult;

  m_context.GenerateByTemplate(answerGenerationTemplate, templateGenResult);

  return answerNodeAddr;
}

bool MessageReplyAgent::linkIsValid(ScAddr const & linkAddr)
{
  if (utils::CommonUtils::checkType(&m_context, linkAddr, ScType::ConstNodeLink))
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
      MessageReplyKeynodes::languages, ScType::VarPermPosArc, ScType::VarNodeClass >> LINK_LANGUAGE_CLASS_ALIAS);
  languageTemplate.Triple(LINK_LANGUAGE_CLASS_ALIAS, ScType::VarPermPosArc, linkAddr);
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(languageTemplate, searchResult);
  return searchResult.Size() == 1;
}

bool MessageReplyAgent::soundLinkIsValid(ScAddr const & linkAddr)
{
  ScTemplate soundLinkTemplate;
  soundLinkTemplate.Triple(MessageReplyKeynodes::concept_sound_file, ScType::VarPermPosArc, linkAddr);
  soundLinkTemplate.Triple(MessageReplyKeynodes::format_wav, ScType::VarPermPosArc, linkAddr);
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(soundLinkTemplate, searchResult);
  return searchResult.Size() == 1;
}

bool MessageReplyAgent::textLinkIsValid(ScAddr const & linkAddr)
{
  ScTemplate textLinkTemplate;
  textLinkTemplate.Triple(MessageReplyKeynodes::concept_text_file, ScType::VarPermPosArc, linkAddr);
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(textLinkTemplate, searchResult);
  return searchResult.Size() == 1;
}
