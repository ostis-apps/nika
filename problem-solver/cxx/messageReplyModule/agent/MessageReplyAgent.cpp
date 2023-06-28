#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "utils/ActionUtils.hpp"
#include "keynodes/Keynodes.hpp"

#include "generator/MessageHistoryGenerator.hpp"

#include "keynodes/MessageReplyKeynodes.hpp"

#include "MessageReplyAgent.hpp"

using namespace messageReplyModule;
using namespace scAgentsCommon;

SC_AGENT_IMPLEMENTATION(MessageReplyAgent)
{
  ScAddr actionAddr = otherAddr;
  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("MessageReplyAgent started");

  ScAddr linkAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, CoreKeynodes::rrel_1);
  ScAddr chatAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, CoreKeynodes::rrel_2);
  ScAddr processingProgramAddr = getMessageProcessingProgram();
  ScAddr authorAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, MessageReplyKeynodes::nrel_authors);
  if (!processingProgramAddr.IsValid())
  {
    SC_LOG_ERROR("Message processing program not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }
  if (!linkIsValid(linkAddr))
  {
    SC_LOG_ERROR("Message link not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }
  if (!chatAddr.IsValid())
  {
    SC_LOG_ERROR("Message chat not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }
  if (!authorAddr.IsValid())
  {
    SC_LOG_ERROR("Message author not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  MessageHistoryGenerator generator = MessageHistoryGenerator(&m_memoryCtx);

  ScAddr messageAddr;
  try
  {
    messageAddr = generateMessage(authorAddr, linkAddr);
    generator.addMessageToDialog(chatAddr, messageAddr);
  }
  catch (std::runtime_error & exception)
  {
    SC_LOG_ERROR(exception.what());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }
  ScAddrVector argsVector = {processingProgramAddr, generateNonAtomicActionArgsSet(messageAddr)};
  ScAddr actionToInterpret = utils::AgentUtils::initAgent(
      &m_memoryCtx, commonModule::Keynodes::action_interpret_non_atomic_action, argsVector);
  ScAddr answerAddr;
  if (!waitForActionSuccessfulFinish(actionToInterpret))
  {
    SC_LOG_ERROR("Action wait time expired or action not finished successfully");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  try
  {
    answerAddr = generateAnswer(messageAddr);

    ScTemplate replySearchTemplate;
    replySearchTemplate.TripleWithRelation(
        messageAddr,
        ScType::EdgeDCommonVar,
        ScType::NodeVar >> "_reply_message",
        ScType::EdgeAccessVarPosPerm,
        MessageReplyKeynodes::nrel_reply);
    ScTemplateSearchResult searchResult;
    m_memoryCtx.HelperSearchTemplate(replySearchTemplate, searchResult);

    ScAddr const & replyMessageAddr = searchResult[0]["_reply_message"];
    utils::GenerationUtils::generateRelationBetween(
        &m_memoryCtx, replyMessageAddr, MessageReplyKeynodes::nika, MessageReplyKeynodes::nrel_authors);

    generator.addMessageToDialog(chatAddr, replyMessageAddr);
  }
  catch (std::runtime_error & exception)
  {
    SC_LOG_ERROR(exception.what());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  SC_LOG_DEBUG("MessageReplyAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerAddr, true);
  return SC_RESULT_OK;
}

bool MessageReplyAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      MessageReplyKeynodes::action_reply_to_message, actionAddr, ScType::EdgeAccessConstPosPerm);
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
  userMessageTemplate.TripleWithRelation(
      USER_MESSAGE_ALIAS,
      ScType::EdgeDCommonVar,
      authorAddr,
      ScType::EdgeAccessVarPosPerm,
      MessageReplyKeynodes::nrel_authors);
  userMessageTemplate.TripleWithRelation(
      ScType::NodeVar >> TRANSLATION_NODE_ALIAS,
      ScType::EdgeDCommonVar,
      USER_MESSAGE_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      CoreKeynodes::nrel_sc_text_translation);
  userMessageTemplate.Triple(TRANSLATION_NODE_ALIAS, ScType::EdgeAccessVarPosPerm, linkAddr);
  ScTemplateGenResult templateGenResult;
  if (!m_memoryCtx.HelperGenTemplate(userMessageTemplate, templateGenResult))
  {
    throw std::runtime_error("Unable to generate message");
  }
  return templateGenResult[USER_MESSAGE_ALIAS];
}

ScAddr MessageReplyAgent::generateNonAtomicActionArgsSet(ScAddr const & messageAddr)
{
  std::string const ARGS_SET_ALIAS = "_args_set";

  ScTemplate argsSetTemplate;
  argsSetTemplate.TripleWithRelation(
      ScType::NodeVar >> ARGS_SET_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      messageAddr,
      ScType::EdgeAccessVarPosPerm,
      CoreKeynodes::rrel_1);
  ScTemplateGenResult templateGenResult;
  if (!m_memoryCtx.HelperGenTemplate(argsSetTemplate, templateGenResult))
  {
    throw std::runtime_error("Unable to generate arguments set for interpreter agent action");
  }
  return templateGenResult[ARGS_SET_ALIAS];
}

ScAddr MessageReplyAgent::generateAnswer(ScAddr const & messageAddr)
{
  std::string const REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS = "_reply_message_relation_pair_arc";
  std::string const REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS = "_reply_message_relation_access_arc";
  std::string const REPLY_MESSAGE_ALIAS = "_reply_message";
  std::string const ANSWER_ALIAS = "_answer";

  ScTemplate replySearchTemplate;
  replySearchTemplate.TripleWithRelation(
      messageAddr,
      ScType::EdgeDCommonVar >> REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS,
      ScType::NodeVar >> REPLY_MESSAGE_ALIAS,
      ScType::EdgeAccessVarPosPerm >> REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS,
      MessageReplyKeynodes::nrel_reply);
  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(replySearchTemplate, searchResult);
  if (searchResult.Size() != 1)
  {
    throw std::runtime_error("Reply message not generated.");
  }

  m_memoryCtx.CreateNode(ScType::NodeConstStruct);
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
  ScIterator3Ptr classesIt = m_memoryCtx.Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, messageAddr);

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
  if (!m_memoryCtx.HelperGenTemplate(answerGenerationTemplate, templateGenResult))
  {
    throw std::runtime_error("Unable to generate answer.");
  }
  return templateGenResult[ANSWER_ALIAS];
}

bool MessageReplyAgent::linkIsValid(ScAddr const & linkAddr)
{
  if (utils::CommonUtils::checkType(&m_memoryCtx, linkAddr, ScType::LinkConst))
  {
    if (!hasLanguage(linkAddr))
      return false;
    bool isSoundValid = soundLinkIsValid(linkAddr);
    bool isTextValid = textLinkIsValid(linkAddr);
    if (isSoundValid)
    {
      SC_LOG_DEBUG("Sound link is found");
      return true;
    }
    if (isTextValid)
    {
      SC_LOG_DEBUG("Text link is found");
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
  m_memoryCtx.HelperSearchTemplate(languageTemplate, searchResult);
  return searchResult.Size() == 1;
}

bool MessageReplyAgent::soundLinkIsValid(ScAddr const & linkAddr)
{
  ScTemplate soundLinkTemplate;
  soundLinkTemplate.Triple(MessageReplyKeynodes::concept_sound_file, ScType::EdgeAccessVarPosPerm, linkAddr);
  soundLinkTemplate.Triple(MessageReplyKeynodes::format_wav, ScType::EdgeAccessVarPosPerm, linkAddr);
  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(soundLinkTemplate, searchResult);
  return searchResult.Size() == 1;
}

bool MessageReplyAgent::textLinkIsValid(ScAddr const & linkAddr)
{
  ScTemplate textLinkTemplate;
  textLinkTemplate.Triple(MessageReplyKeynodes::concept_text_file, ScType::EdgeAccessVarPosPerm, linkAddr);
  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(textLinkTemplate, searchResult);
  return searchResult.Size() == 1;
}

bool MessageReplyAgent::waitForActionSuccessfulFinish(ScAddr const & actionAddr)
{
  return ActionUtils::waitAction(&m_memoryCtx, actionAddr, WAIT_TIME) &&
         m_memoryCtx.HelperCheckEdge(
             CoreKeynodes::question_finished_successfully, actionAddr, ScType::EdgeAccessConstPosPerm);
}
