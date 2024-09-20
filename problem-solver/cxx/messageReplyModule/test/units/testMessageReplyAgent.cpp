#include "sc_test.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include <sc-memory/sc_agent.hpp>
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "agent/MessageReplyAgent.hpp"
#include "keynodes/Keynodes.hpp"
#include "keynodes/MessageReplyKeynodes.hpp"
#include "test/agent/GenerateReplyMessageAgent.hpp"
#include "utils/ActionUtils.hpp"

namespace messageReplyModuleTest
{
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = MESSAGE_REPLY_MODULE_TEST_SRC_PATH "/testStructures/";
const int WAIT_TIME = 5000;

using MessageReplyAgentTest = ScMemoryTest;

void initialize()
{
  ScKeynodes::InitGlobal();
  commonModule::Keynodes::InitGlobal();
  messageReplyModule::MessageReplyKeynodes::InitGlobal();

  ScAgentInit(true);
  //todo(codegen-removal): Use agentContext.SubscribeAgent<messageReplyModule::MessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(messageReplyModule::MessageReplyAgent)
}

void shutdown()
{
  //todo(codegen-removal): Use agentContext.SubscribeAgent<messageReplyModule::MessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(messageReplyModule::MessageReplyAgent)
}

bool generatedMessageIsValid(ScMemoryContext * context, ScAddr const & soundLinkAddr)
{
  ScTemplate scTemplate;
  scTemplate.Triple(
      messageReplyModule::MessageReplyKeynodes::concept_message,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_user_message");
  scTemplate.TripleWithRelation(
      "_user_message",
      ScType::EdgeDCommonVar,
      ScType::NodeVar,
      ScType::EdgeAccessVarPosPerm,
      messageReplyModule::MessageReplyKeynodes::nrel_authors);
  scTemplate.TripleWithRelation(
      ScType::NodeVar >> "_translation_node",
      ScType::EdgeDCommonVar,
      "_user_message",
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::nrel_sc_text_translation);
  scTemplate.Triple(
      "_translation_node",
      ScType::EdgeAccessVarPosPerm,
      soundLinkAddr);
  ScTemplateSearchResult searchResult;
  context->SearchByTemplate(scTemplate, searchResult);
  return searchResult.Size() == 1;
}

TEST_F(MessageReplyAgentTest, messageProcessingWithTextLinkSuccessful)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(
          context,
          TEST_FILES_DIR_PATH + "replyMessageAgentTextLinkTestStructure.scs");
  initialize();
  //todo(codegen-removal): Use agentContext.SubscribeAgent<GenerateReplyMessageAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(GenerateReplyMessageAgent)

  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(test_action_node.IsValid());

  context.GenerateConnector(
          ScType::EdgeAccessConstPosPerm,
          ScKeynodes::question_initiated,
          test_action_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
          ScKeynodes::question_finished_successfully,
          test_action_node,
          ScType::EdgeAccessConstPosPerm));

  EXPECT_TRUE(generatedMessageIsValid(&context, utils::IteratorUtils::getFirstByOutRelation(
          &context,
          test_action_node,
          ScKeynodes::rrel_1)));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<GenerateReplyMessageAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(GenerateReplyMessageAgent)
  shutdown();
}

TEST_F(MessageReplyAgentTest, messageProcessingWithSoundLinkSuccessful)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(
          context,
          TEST_FILES_DIR_PATH + "replyMessageAgentSoundLinkTestStructure.scs");
  initialize();
  //todo(codegen-removal): Use agentContext.SubscribeAgent<GenerateReplyMessageAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(GenerateReplyMessageAgent)

  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(test_action_node.IsValid());

  context.GenerateConnector(
          ScType::EdgeAccessConstPosPerm,
          ScKeynodes::question_initiated,
          test_action_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
          ScKeynodes::question_finished_successfully,
          test_action_node,
          ScType::EdgeAccessConstPosPerm));

  EXPECT_TRUE(generatedMessageIsValid(&context, utils::IteratorUtils::getFirstByOutRelation(
          &context,
          test_action_node,
          ScKeynodes::rrel_1)));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<GenerateReplyMessageAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(GenerateReplyMessageAgent)
  shutdown();
}

TEST_F(MessageReplyAgentTest, argumentIsNotALink)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(
      context,
      TEST_FILES_DIR_PATH + "replyMessageAgentTestStructureFirstArgumentIsNotALink.scs");
  initialize();
  //todo(codegen-removal): Use agentContext.SubscribeAgent<GenerateReplyMessageAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(GenerateReplyMessageAgent)

  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(test_action_node.IsValid());

  context.GenerateConnector(
      ScType::EdgeAccessConstPosPerm,
      ScKeynodes::question_initiated,
      test_action_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
      ScKeynodes::question_finished_unsuccessfully,
      test_action_node,
      ScType::EdgeAccessConstPosPerm));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<GenerateReplyMessageAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(GenerateReplyMessageAgent)
  shutdown();
}

TEST_F(MessageReplyAgentTest, linkSpecifiedIncorrectly)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(
      context,
      TEST_FILES_DIR_PATH + "replyMessageAgentTestStructureWithIncorrectlySpecifiedLink.scs");
  initialize();
  //todo(codegen-removal): Use agentContext.SubscribeAgent<GenerateReplyMessageAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(GenerateReplyMessageAgent)

  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(test_action_node.IsValid());

  context.GenerateConnector(
      ScType::EdgeAccessConstPosPerm,
      ScKeynodes::question_initiated,
      test_action_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
      ScKeynodes::question_finished_unsuccessfully,
      test_action_node,
      ScType::EdgeAccessConstPosPerm));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<GenerateReplyMessageAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(GenerateReplyMessageAgent)
  shutdown();
}

}//namespace messageReplyModuleTest
