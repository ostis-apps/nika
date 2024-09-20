#include <sc-memory/sc_agent.hpp>

#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include "sc_test.hpp"

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

void initialize(ScAgentContext & context)
{
  context.SubscribeAgent<messageReplyModule::MessageReplyAgent>();
}

void shutdown(ScAgentContext & context)
{
  context.UnsubscribeAgent<messageReplyModule::MessageReplyAgent>();
}

bool generatedMessageIsValid(ScMemoryContext * context, ScAddr const & soundLinkAddr)
{
  ScTemplate scTemplate;
  scTemplate.Triple(
      messageReplyModule::MessageReplyKeynodes::concept_message,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_user_message");
  scTemplate.Quintuple(
      "_user_message",
      ScType::EdgeDCommonVar,
      ScType::NodeVar,
      ScType::EdgeAccessVarPosPerm,
      messageReplyModule::MessageReplyKeynodes::nrel_authors);
  scTemplate.Quintuple(
      ScType::NodeVar >> "_translation_node",
      ScType::EdgeDCommonVar,
      "_user_message",
      ScType::EdgeAccessVarPosPerm,
      commonModule::Keynodes::nrel_sc_text_translation);
  scTemplate.Triple("_translation_node", ScType::EdgeAccessVarPosPerm, soundLinkAddr);
  ScTemplateSearchResult searchResult;
  context->SearchByTemplate(scTemplate, searchResult);
  return searchResult.Size() == 1;
}

TEST_F(MessageReplyAgentTest, messageProcessingWithTextLinkSuccessful)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "replyMessageAgentTextLinkTestStructure.scs");

  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction action = context.ConvertToAction(test_action_node);
  EXPECT_TRUE(test_action_node.IsValid());
  initialize(context);

  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  //EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
      ScKeynodes::action_finished_successfully, test_action_node, ScType::EdgeAccessConstPosPerm));

  EXPECT_TRUE(generatedMessageIsValid(
      &context, utils::IteratorUtils::getAnyByOutRelation(&context, test_action_node, ScKeynodes::rrel_1)));

  shutdown(context);
}

TEST_F(MessageReplyAgentTest, messageProcessingWithSoundLinkSuccessful)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "replyMessageAgentSoundLinkTestStructure.scs");

  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(test_action_node.IsValid());

  initialize(context);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
      ScKeynodes::action_finished_successfully, test_action_node, ScType::EdgeAccessConstPosPerm));

  EXPECT_TRUE(generatedMessageIsValid(
      &context, utils::IteratorUtils::getAnyByOutRelation(&context, test_action_node, ScKeynodes::rrel_1)));

  shutdown(context);
}

TEST_F(MessageReplyAgentTest, argumentIsNotALink)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "replyMessageAgentTestStructureFirstArgumentIsNotALink.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");

  EXPECT_TRUE(test_action_node.IsValid());

  initialize(context);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
      ScKeynodes::action_finished_unsuccessfully, test_action_node, ScType::EdgeAccessConstPosPerm));

  shutdown(context);
}

TEST_F(MessageReplyAgentTest, linkSpecifiedIncorrectly)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "replyMessageAgentTestStructureWithIncorrectlySpecifiedLink.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");

  EXPECT_TRUE(test_action_node.IsValid());

  initialize(context);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
      ScKeynodes::action_finished_unsuccessfully, test_action_node, ScType::EdgeAccessConstPosPerm));
  shutdown(context);
}

}  // namespace messageReplyModuleTest
