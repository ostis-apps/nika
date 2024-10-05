#include <sc-memory/sc_agent.hpp>

#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include "sc_test.hpp"

#include "agent/FindWordInSetByFirstLetterAgent.hpp"
#include "utils/ActionUtils.hpp"

namespace messageProcessingModuleTest
{
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = MESSAGE_PROCESSING_MODULE_TEST_SRC_PATH "/testStructures/";
const int WAIT_TIME = 5000;

using MessageProcessingAgentTest = ScMemoryTest;

TEST_F(MessageProcessingAgentTest, notValidMessageTest)
{
  ScAgentContext & ctx = *m_ctx;
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "notValidMessageTest.scs");
  ScAddr testActionNode = ctx.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(
      ctx.CheckConnector(ScKeynodes::action_finished_unsuccessfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ctx.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();
}

TEST_F(MessageProcessingAgentTest, messageIsNotAboutLetterSearchTest)
{
  ScAgentContext & ctx = *m_ctx;
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "messageIsNotAboutLetterSearch.scs");
  ScAddr testActionNode = ctx.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(
      ctx.CheckConnector(ScKeynodes::action_finished_unsuccessfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ctx.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithSingleWordTest1)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generateAnswerWithSingleWordTest1.scs");
  ScAddr testActionNode = ctx.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(
      ctx.CheckConnector(ScKeynodes::action_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ScAddr answerAddr = ctx.SearchElementBySystemIdentifier("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&ctx, answerAddr);
  std::string linkContent;
  ctx.GetLinkContent(answerLink, linkContent);
  EXPECT_EQ(linkContent, "Апельсин");

  ctx.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithSingleWordTest2)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generateAnswerWithSingleWordTest2.scs");

  ScAddr testActionNode = ctx.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(
      ctx.CheckConnector(ScKeynodes::action_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ScAddr answerAddr = ctx.SearchElementBySystemIdentifier("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&ctx, answerAddr);
  std::string linkContent;
  ctx.GetLinkContent(answerLink, linkContent);
  EXPECT_EQ(linkContent, "Мандарин");

  ctx.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithSeveralWordsTest)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generateAnswerWithSeveralWordsTest.scs");
  ScAddr testActionNode = ctx.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(
      ctx.CheckConnector(ScKeynodes::action_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ScAddr answerAddr = ctx.SearchElementBySystemIdentifier("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&ctx, answerAddr);
  std::string linkContent;
  ctx.GetLinkContent(answerLink, linkContent);
  EXPECT_EQ(linkContent, "Ананас, Апельсин");

  ctx.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithoutWords)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generateAnswerWithoutWords.scs");

  ScAddr testActionNode = ctx.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(
      ctx.CheckConnector(ScKeynodes::action_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ScAddr answerAddr = ctx.SearchElementBySystemIdentifier("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&ctx, answerAddr);
  std::string linkContent;
  ctx.GetLinkContent(answerLink, linkContent);
  EXPECT_EQ(linkContent, "Таких слов нет");

  ctx.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();
}

}  // namespace messageProcessingModuleTest
