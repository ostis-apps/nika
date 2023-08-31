#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-memory/sc_wait.hpp"
#include "sc_test.hpp"

#include "agent/FindWordInSetByFirstLetterAgent.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "keynodes/MessageProcessingKeynodes.hpp"
#include "utils/ActionUtils.hpp"

namespace messageProcessingModuleTest
{
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = MESSAGE_PROCESSING_MODULE_TEST_SRC_PATH "/testStructures/";
const int WAIT_TIME = 5000;

using MessageProcessingAgentTest = ScMemoryTest;

void initialize()
{
  scAgentsCommon::CoreKeynodes::InitGlobal();
  messageProcessingModule::MessageProcessingKeynodes::InitGlobal();
  dialogControlModule::MessageKeynodes::InitGlobal();

  ScAgentInit(true);
  SC_AGENT_REGISTER(messageProcessingModule::FindWordInSetByFirstLetterAgent)
}

void shutdown(){SC_AGENT_UNREGISTER(messageProcessingModule::FindWordInSetByFirstLetterAgent)}

TEST_F(MessageProcessingAgentTest, notValidMessageTest)
{
  ScMemoryContext & ctx = *m_ctx;
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "notValidMessageTest.scs");
  initialize();

  ScAddr testActionNode = ctx.HelperFindBySystemIdtf("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(ctx.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  shutdown();
}

TEST_F(MessageProcessingAgentTest, messageIsNotAboutLetterSearchTest)
{
  ScMemoryContext & ctx = *m_ctx;
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "messageIsNotAboutLetterSearch.scs");
  initialize();

  ScAddr testActionNode = ctx.HelperFindBySystemIdtf("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(ctx.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  shutdown();
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithSingleWordTest1)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generateAnswerWithSingleWordTest1.scs");
  initialize();

  ScAddr testActionNode = ctx.HelperFindBySystemIdtf("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(ctx.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ScAddr answerAddr = ctx.HelperFindBySystemIdtf("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&ctx, answerAddr);
  EXPECT_EQ(utils::CommonUtils::getLinkContent(&ctx, answerLink), "Апельсин");

  shutdown();
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithSingleWordTest2)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generateAnswerWithSingleWordTest2.scs");
  initialize();

  ScAddr testActionNode = ctx.HelperFindBySystemIdtf("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(ctx.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ScAddr answerAddr = ctx.HelperFindBySystemIdtf("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&ctx, answerAddr);
  EXPECT_EQ(utils::CommonUtils::getLinkContent(&ctx, answerLink), "Мандарин");

  shutdown();
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithSeveralWordsTest)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generateAnswerWithSeveralWordsTest.scs");
  initialize();

  ScAddr testActionNode = ctx.HelperFindBySystemIdtf("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(ctx.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ScAddr answerAddr = ctx.HelperFindBySystemIdtf("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&ctx, answerAddr);
  EXPECT_EQ(utils::CommonUtils::getLinkContent(&ctx, answerLink), "Ананас, Апельсин");

  shutdown();
}

TEST_F(MessageProcessingAgentTest, generateAnswerWithoutWords)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generateAnswerWithoutWords.scs");
  initialize();

  ScAddr testActionNode = ctx.HelperFindBySystemIdtf("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);

  EXPECT_TRUE(ActionUtils::waitAction(&ctx, testActionNode, WAIT_TIME));
  EXPECT_TRUE(ctx.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ScAddr answerAddr = ctx.HelperFindBySystemIdtf("word_starts_with_required_letter_answer_phrase");
  EXPECT_TRUE(answerAddr.IsValid());

  ScAddr answerLink = utils::IteratorUtils::getAnyFromSet(&ctx, answerAddr);
  EXPECT_EQ(utils::CommonUtils::getLinkContent(&ctx, answerLink), "Таких слов нет");

  shutdown();
}

}  // namespace messageProcessingModuleTest
