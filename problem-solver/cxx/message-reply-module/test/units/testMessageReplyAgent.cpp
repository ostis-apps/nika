#include <sc-memory/sc_agent.hpp>
#include <sc-memory/test/sc_test.hpp>

#include "agent/GenerateReplyMessageAgent.hpp"
#include "agent/MessageReplyAgent.hpp"
#include "keynodes/MessageReplyKeynodes.hpp"
#include <sc-builder/scs_loader.hpp>

namespace messageReplyModuleTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = "../test-structures/";
int const WAIT_TIME = 5000;

using MessageReplyAgentTest = ScMemoryTest;

void initialize(ScAgentContext & context)
{
  context.SubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.SubscribeAgent<GenerateReplyMessageAgent>();
}

void shutdown(ScAgentContext & context)
{
  context.UnsubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.UnsubscribeAgent<GenerateReplyMessageAgent>();
}

bool generatedMessageIsValid(ScMemoryContext * context, ScAddr const & soundLinkAddr)
{
  ScTemplate scTemplate;
  scTemplate.Triple(
      messageReplyModule::MessageReplyKeynodes::concept_message,
      ScType::VarPermPosArc,
      ScType::VarNode >> "_user_message");
  scTemplate.Quintuple(
      "_user_message",
      ScType::VarCommonArc,
      ScType::VarNode,
      ScType::VarPermPosArc,
      messageReplyModule::MessageReplyKeynodes::nrel_authors);
  scTemplate.Quintuple(
      ScType::VarNode >> "_translation_node",
      ScType::VarCommonArc,
      "_user_message",
      ScType::VarPermPosArc,
      messageReplyModule::MessageReplyKeynodes::nrel_sc_text_translation);
  scTemplate.Triple("_translation_node", ScType::VarPermPosArc, soundLinkAddr);
  ScTemplateSearchResult searchResult;
  context->SearchByTemplate(scTemplate, searchResult);
  return searchResult.Size() == 1;
}

TEST_F(MessageReplyAgentTest, messageProcessingWithTextLinkSuccessful)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "replyMessageAgentTextLinkTestStructure.scs");

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  initialize(context);

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  EXPECT_TRUE(generatedMessageIsValid(&context, testAction.GetArgument(ScKeynodes::rrel_1)));

  shutdown(context);
}

TEST_F(MessageReplyAgentTest, messageProcessingWithSoundLinkSuccessful)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "replyMessageAgentSoundLinkTestStructure.scs");

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  initialize(context);

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  EXPECT_TRUE(generatedMessageIsValid(&context, testAction.GetArgument(ScKeynodes::rrel_1)));

  shutdown(context);
}

TEST_F(MessageReplyAgentTest, argumentIsNotALink)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "replyMessageAgentTestStructureFirstArgumentIsNotALink.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");

  EXPECT_TRUE(testActionNode.IsValid());

  initialize(context);

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());

  shutdown(context);
}

TEST_F(MessageReplyAgentTest, linkSpecifiedIncorrectly)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "replyMessageAgentTestStructureWithIncorrectlySpecifiedLink.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");

  EXPECT_TRUE(testActionNode.IsValid());

  initialize(context);

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());

  shutdown(context);
}

}  // namespace messageReplyModuleTest
