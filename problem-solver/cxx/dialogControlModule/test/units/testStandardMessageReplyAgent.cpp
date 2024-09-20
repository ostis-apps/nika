#include "agent/DirectInferenceAgent.hpp"
#include "agent/StandardMessageReplyAgent.hpp"
#include "agent/PhraseGenerationAgent.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include "keynodes/DialogKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include <sc-memory/sc_agent.hpp>
#include "sc_test.hpp"
#include "searcher/TokenDomainSearcher.hpp"

using namespace inference;
using namespace dialogControlModule;

namespace testStandardMessageReplyAgent
{
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = DIALOG_CONTROL_MODULE_TEST_SRC_PATH "/TestsStructures/StandardMessageReply/";
const std::string ACTION_DIR_PATH = "actions/";
const std::string LR_DIR_PATH = "logicRules/";
const std::string MES_DIR_PATH = "messages/";
const std::string PHRASE_DIR_PATH = "phrases/";
const int WAIT_TIME = 20000;

using StandardMessageReplyTest = ScMemoryTest;

void initializeClasses()
{
  DialogKeynodes::InitGlobal();
  InferenceKeynodes::InitGlobal();
  commonModule::Keynodes::InitGlobal();
  MessageKeynodes::InitGlobal();
  ScKeynodes::InitGlobal();
  StandardMessageReplyAgent::InitGlobal();
}


TEST_F(StandardMessageReplyTest, ProcessingAtomicMessageIsSuccessful)
{
  string messageText = "I'm sorry, Mister Ivanov.";
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr4.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH +
                          "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");
  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(PhraseGenerationAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)

  ctx.GenerateConnector(
          ScType::EdgeAccessConstPosPerm,
          ScKeynodes::question_initiated,
          test_action_node);

  EXPECT_TRUE(
          ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
                  ctx,
                  ScKeynodes::question_finished_successfully).
                  Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(PhraseGenerationAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(DirectInferenceAgent)

  ScAddr messageNode = ctx.SearchElementBySystemIdentifier("test_message");
  EXPECT_TRUE(messageNode.IsValid());

  ScAddr replyMessageNode = utils::IteratorUtils::getFirstByOutRelation(&ctx, messageNode, MessageKeynodes::nrel_reply);
  EXPECT_TRUE(replyMessageNode.IsValid());

  TokenDomainSearcher searcher(&ctx);
  ScAddr messageLink = searcher.getMessageText(replyMessageNode);
  EXPECT_EQ(utils::CommonUtils::getLinkContent(&ctx, messageLink), messageText);
}

TEST_F(StandardMessageReplyTest, ProcessingNonatomicMessageIsSuccessful)
{
  string messageText = "I'm sorry, Mister Ivanov. How did this happen, Ivan?";
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH +
                          "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase2.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(PhraseGenerationAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)

  ctx.GenerateConnector(
          ScType::EdgeAccessConstPosPerm,
          ScKeynodes::question_initiated,
          test_action_node);

  EXPECT_TRUE(
          ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
                  ctx,
                  ScKeynodes::question_finished_successfully).
                  Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(PhraseGenerationAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(DirectInferenceAgent)

  ScAddr messageNode = ctx.SearchElementBySystemIdentifier("test_message");
  EXPECT_TRUE(messageNode.IsValid());

  ScAddr replyMessageNode = utils::IteratorUtils::getFirstByOutRelation(&ctx, messageNode, MessageKeynodes::nrel_reply);
  EXPECT_TRUE(replyMessageNode.IsValid());

  TokenDomainSearcher searcher(&ctx);
  ScAddr messageLink = searcher.getMessageText(replyMessageNode);
  EXPECT_EQ(utils::CommonUtils::getLinkContent(&ctx, messageLink), messageText);
}

TEST_F(StandardMessageReplyTest, ActionDoesntHaveAMessageNode)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "incorrectAction.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
}

TEST_F(StandardMessageReplyTest, SystemDoesNotHaveTemplateForMessage)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH +
                          "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message1.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(DirectInferenceAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
}

TEST_F(StandardMessageReplyTest, MessagesLanguageIsNotFound)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH +
                          "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message2.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(DirectInferenceAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
}

TEST_F(StandardMessageReplyTest, FirstMessageIsNotFound)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr2.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH +
                          "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(DirectInferenceAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
}

TEST_F(StandardMessageReplyTest, FirstPhraseClassIsNotFound)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr3.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH +
                          "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(DirectInferenceAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
}

TEST_F(StandardMessageReplyTest, PhraseForMessageIsNotFound)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH +
                          "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(DirectInferenceAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
}

TEST_F(StandardMessageReplyTest, LinkByPhraseIsNotGenerated)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH +
                          "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");
  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(PhraseGenerationAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
}

TEST_F(StandardMessageReplyTest, MessagesAndPhraseClassesDoNotMatch1)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr5.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH +
                          "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");
  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(PhraseGenerationAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
}

TEST_F(StandardMessageReplyTest, MessagesAndPhraseClassesDoNotMatch2)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr6.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH +
                          "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");
  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(StandardMessageReplyAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(PhraseGenerationAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<DirectInferenceAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(DirectInferenceAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(StandardMessageReplyAgent)
}
}
