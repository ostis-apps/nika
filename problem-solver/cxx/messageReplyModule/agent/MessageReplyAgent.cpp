#include "MessageReplyAgent.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "agent/NonAtomicActionInterpreterAgent.hpp"
#include "generator/MessageHistoryGenerator.hpp"
#include "keynodes/Keynodes.hpp"
#include "keynodes/MessageReplyKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace messageReplyModule;
// todo(codegen-removal): remove agent starting and finishing logs, sc-machine is printing them now
// todo(codegen-removal): if your agent is ScActionInitiatedAgent and uses event only to get action node via
// event.GetOtherElement() then you can remove event from method arguments and use ScAction & action instead of your
// action node todo(codegen-removal): if your agent is having method like CheckActionClass(ScAddr actionAddr) that
// checks connector between action class and actionAddr then you can remove it. Before agent is started sc-machine check
// that action belongs to class returned by GetActionClass() todo(codegen-removal): use action.SetResult() to pass
// result of your action instead of using answer or answerElements todo(codegen-removal): use SC_AGENT_LOG_SOMETHING()
// instead of SC_LOG_SOMETHING to automatically include agent name to logs messages todo(codegen-removal): use auto
// const & [names of action arguments] = action.GetArguments<amount of arguments>(); to get action arguments
ScResult MessageReplyAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr linkAddr = utils::IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_1);
  ScAddr chatAddr = utils::IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_2);
  ScAddr processingProgramAddr = getMessageProcessingProgram();
  ScAddr authorAddr = utils::IteratorUtils::getAnyByOutRelation(&m_context, action, MessageReplyKeynodes::nrel_authors);
  if (!processingProgramAddr.IsValid())
  {
    SC_AGENT_LOG_ERROR("Message processing program not found.");
    return action.FinishUnsuccessfully();
  }
  if (!linkIsValid(linkAddr))
  {
    SC_AGENT_LOG_ERROR("Message link not found.");
    return action.FinishUnsuccessfully();
  }
  if (!chatAddr.IsValid())
  {
    SC_AGENT_LOG_ERROR("Message chat not found.");
    return action.FinishUnsuccessfully();
  }
  if (!authorAddr.IsValid())
  {
    SC_AGENT_LOG_ERROR("Message author not found.");
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
    SC_LOG_ERROR(exception.what());
    return action.FinishUnsuccessfully();
  }
  ScAddrVector argsVector = {processingProgramAddr, generateNonAtomicActionArgsSet(messageAddr)};
  // todo(codegen-removal): replace AgentUtils:: usage
  ScAddr actionToInterpret = m_context.GenerateNode(ScType::NodeConst);
  m_context.GenerateConnector(
      ScType::EdgeAccessConstPosPerm, MessageReplyKeynodes::action_reply_to_message, actionToInterpret);
  m_context.SubscribeAgent<commonModule::NonAtomicActionInterpreterAgent>();

  ScAddr answerAddr;
  if (!waitForActionSuccessfulFinish(actionToInterpret))
  {
    SC_AGENT_LOG_ERROR("Action wait time expired or action not finished successfully");
    return action.FinishUnsuccessfully();
  }

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
    SC_AGENT_LOG_ERROR(exception.what());
    return action.FinishUnsuccessfully();
  }

  SC_AGENT_LOG_DEBUG("MessageReplyAgent finished");
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
  std::string const USER_MESSAGE_ALIAS = "_user_message";
  std::string const TRANSLATION_NODE_ALIAS = "_translation_node";

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
      commonModule::Keynodes::nrel_sc_text_translation);
  userMessageTemplate.Triple(TRANSLATION_NODE_ALIAS, ScType::EdgeAccessVarPosPerm, linkAddr);
  ScTemplateGenResult templateGenResult;

  m_context.GenerateByTemplate(userMessageTemplate, templateGenResult);

  return templateGenResult[USER_MESSAGE_ALIAS];
}

ScAddr MessageReplyAgent::generateNonAtomicActionArgsSet(ScAddr const & messageAddr)
{
  std::string const ARGS_SET_ALIAS = "_args_set";

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
  std::string const REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS = "_reply_message_relation_pair_arc";
  std::string const REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS = "_reply_message_relation_access_arc";
  std::string const REPLY_MESSAGE_ALIAS = "_reply_message";
  std::string const ANSWER_ALIAS = "_answer";

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

  m_context.GenerateNode(ScType::NodeConstStruct);
  ScTemplate answerGenerationTemplate;
  answerGenerationTemplate.Triple(ScType::NodeVarStruct >> ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, messageAddr);
  answerGenerationTemplate.Triple(
      ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, searchResult[0][REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS]);
  answerGenerationTemplate.Triple(ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, searchResult[0][REPLY_MESSAGE_ALIAS]);
  answerGenerationTemplate.Triple(
      ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, searchResult[0][REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS]);
  answerGenerationTemplate.Triple(ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, MessageReplyKeynodes::nrel_reply);

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
    answerGenerationTemplate.Triple(ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, classes.at(i));
    answerGenerationTemplate.Triple(ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, classesArcs.at(i));
  }

  ScTemplateGenResult templateGenResult;

  m_context.GenerateByTemplate(answerGenerationTemplate, templateGenResult);

  return templateGenResult[ANSWER_ALIAS];
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
      SC_AGENT_LOG_DEBUG("Sound link is found");
      return true;
    }
    if (isTextValid)
    {
      SC_AGENT_LOG_DEBUG("Text link is found");
      return true;
    }
  }
  return false;
}

bool MessageReplyAgent::hasLanguage(ScAddr const & linkAddr)
{
  string link_language_class = "_link_language_class";

  ScTemplate languageTemplate;
  languageTemplate.Triple(
      MessageReplyKeynodes::languages, ScType::EdgeAccessVarPosPerm, ScType::NodeVarClass >> link_language_class);
  languageTemplate.Triple(link_language_class, ScType::EdgeAccessVarPosPerm, linkAddr);
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

bool MessageReplyAgent::waitForActionSuccessfulFinish(ScAddr const & actionAddr)
{
  return ActionUtils::waitAction(&m_context, actionAddr, WAIT_TIME) &&
         m_context.CheckConnector(ScKeynodes::action_finished_successfully, actionAddr, ScType::EdgeAccessConstPosPerm);
}
