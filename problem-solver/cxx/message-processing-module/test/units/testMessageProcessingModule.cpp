#include <sc-memory/sc_agent.hpp>
#include <sc-memory/test/sc_test.hpp>

#include "agent/FindWordInSetByFirstLetterAgent.hpp"
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-builder/scs_loader.hpp>

namespace messageProcessingModuleTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = "../test-structures/";
int const WAIT_TIME = 5000;

using MessageProcessingAgentTest = ScMemoryTest;

TEST_F(MessageProcessingAgentTest, notValidMessageTest)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "notValidMessageTest.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  context.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());

  context.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();
}

TEST_F(MessageProcessingAgentTest, messageIsNotAboutLetterSearchTest)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "messageIsNotAboutLetterSearch.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  context.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());

  context.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithSingleWordTest1)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "generateAnswerWithSingleWordTest1.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  context.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  context.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  ScAddr answerAddr = context.SearchElementBySystemIdentifier("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&context, answerAddr);
  std::string linkContent;
  context.GetLinkContent(answerLink, linkContent);
  EXPECT_EQ(linkContent, "Апельсин");
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithSingleWordTest2)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "generateAnswerWithSingleWordTest2.scs");

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  context.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  context.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  ScAddr answerAddr = context.SearchElementBySystemIdentifier("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&context, answerAddr);
  std::string linkContent;
  context.GetLinkContent(answerLink, linkContent);
  EXPECT_EQ(linkContent, "Мандарин");
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithSeveralWordsTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "generateAnswerWithSeveralWordsTest.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  context.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  context.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  ScAddr answerAddr = context.SearchElementBySystemIdentifier("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&context, answerAddr);
  std::string linkContent;
  context.GetLinkContent(answerLink, linkContent);
  EXPECT_EQ(linkContent, "Ананас, Апельсин");
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithoutWords)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "generateAnswerWithoutWords.scs");

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  context.SubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  context.UnsubscribeAgent<messageProcessingModule::FindWordInSetByFirstLetterAgent>();

  ScAddr answerAddr = context.SearchElementBySystemIdentifier("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&context, answerAddr);
  std::string linkContent;
  context.GetLinkContent(answerLink, linkContent);
  EXPECT_EQ(linkContent, "Таких слов нет");
}

}  // namespace messageProcessingModuleTest
