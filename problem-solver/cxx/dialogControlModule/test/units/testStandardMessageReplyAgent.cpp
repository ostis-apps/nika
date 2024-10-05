#include <sc-memory/sc_agent.hpp>

#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include "sc_test.hpp"

#include "agent/DirectInferenceAgent.hpp"
#include "agent/PhraseGenerationAgent.hpp"
#include "agent/StandardMessageReplyAgent.hpp"
#include "keynodes/MessageKeynodes.hpp"
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

void initialize(ScAgentContext & ctx)
{
  ctx.SubscribeAgent<DirectInferenceAgent>();
  ctx.SubscribeAgent<PhraseGenerationAgent>();
  ctx.SubscribeAgent<StandardMessageReplyAgent>();
}

void shutdown(ScAgentContext & ctx)
{
  ctx.UnsubscribeAgent<DirectInferenceAgent>();
  ctx.UnsubscribeAgent<PhraseGenerationAgent>();
  ctx.UnsubscribeAgent<StandardMessageReplyAgent>();
}

TEST_F(StandardMessageReplyTest, ProcessingAtomicMessageIsSuccessful)
{
  string messageText = "I'm sorry, Mister Ivanov.";
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr4.scs");
  loader.loadScsFile(
      ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");

  ScAction action = ctx.ConvertToAction(test_action_node);

  initialize(ctx);

  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));

  ScAddr messageNode = ctx.SearchElementBySystemIdentifier("test_message");
  EXPECT_TRUE(messageNode.IsValid());

  ScAddr replyMessageNode = utils::IteratorUtils::getAnyByOutRelation(&ctx, messageNode, MessageKeynodes::nrel_reply);
  EXPECT_TRUE(replyMessageNode.IsValid());

  TokenDomainSearcher searcher(&ctx);
  ScAddr messageLink = searcher.getMessageText(replyMessageNode);
  std::string linkContent;
  ctx.GetLinkContent(messageLink, linkContent);
  EXPECT_EQ(linkContent, messageText);

  shutdown(ctx);
}

TEST_F(StandardMessageReplyTest, ProcessingNonatomicMessageIsSuccessful)
{
  string messageText = "I'm sorry, Mister Ivanov. How did this happen, Ivan?";
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(
      ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase2.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");

  ScAction action = ctx.ConvertToAction(test_action_node);

  initialize(ctx);
  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  shutdown(ctx);

  ScAddr messageNode = ctx.SearchElementBySystemIdentifier("test_message");
  EXPECT_TRUE(messageNode.IsValid());

  ScAddr replyMessageNode = utils::IteratorUtils::getAnyByOutRelation(&ctx, messageNode, MessageKeynodes::nrel_reply);
  EXPECT_TRUE(replyMessageNode.IsValid());

  TokenDomainSearcher searcher(&ctx);
  ScAddr messageLink = searcher.getMessageText(replyMessageNode);
  std::string linkContent;
  ctx.GetLinkContent(messageLink, linkContent);
  EXPECT_EQ(linkContent, messageText);
}

TEST_F(StandardMessageReplyTest, ActionDoesntHaveAMessageNode)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "incorrectAction.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction action = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<StandardMessageReplyAgent>();
  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinishedWithError());
  ctx.UnsubscribeAgent<StandardMessageReplyAgent>();
}

TEST_F(StandardMessageReplyTest, SystemDoesNotHaveTemplateForMessage)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(
      ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message1.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction action = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<StandardMessageReplyAgent>();
  ctx.SubscribeAgent<DirectInferenceAgent>();

  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinishedWithError());

  ctx.UnsubscribeAgent<DirectInferenceAgent>();
  ctx.UnsubscribeAgent<StandardMessageReplyAgent>();
}

TEST_F(StandardMessageReplyTest, MessagesLanguageIsNotFound)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(
      ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message2.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction action = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<StandardMessageReplyAgent>();
  ctx.SubscribeAgent<DirectInferenceAgent>();
  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinishedUnsuccessfully());
  ctx.UnsubscribeAgent<DirectInferenceAgent>();
  ctx.UnsubscribeAgent<StandardMessageReplyAgent>();
}

TEST_F(StandardMessageReplyTest, FirstMessageIsNotFound)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr2.scs");
  loader.loadScsFile(
      ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction action = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<StandardMessageReplyAgent>();
  ctx.SubscribeAgent<DirectInferenceAgent>();
  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinishedUnsuccessfully());
  ctx.UnsubscribeAgent<StandardMessageReplyAgent>();
  ctx.UnsubscribeAgent<DirectInferenceAgent>();
}

TEST_F(StandardMessageReplyTest, FirstPhraseClassIsNotFound)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr3.scs");
  loader.loadScsFile(
      ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction action = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<StandardMessageReplyAgent>();
  ctx.SubscribeAgent<DirectInferenceAgent>();

  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinishedUnsuccessfully());

  ctx.UnsubscribeAgent<StandardMessageReplyAgent>();
  ctx.UnsubscribeAgent<DirectInferenceAgent>();
}

TEST_F(StandardMessageReplyTest, PhraseForMessageIsNotFound)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(
      ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction action = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<StandardMessageReplyAgent>();
  ctx.SubscribeAgent<DirectInferenceAgent>();
  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinishedUnsuccessfully());
  ctx.UnsubscribeAgent<StandardMessageReplyAgent>();
  ctx.UnsubscribeAgent<DirectInferenceAgent>();
}

TEST_F(StandardMessageReplyTest, LinkByPhraseIsNotGenerated)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(
      ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction action = ctx.ConvertToAction(test_action_node);
  initialize(ctx);
  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinishedUnsuccessfully());
  shutdown(ctx);
}

TEST_F(StandardMessageReplyTest, MessagesAndPhraseClassesDoNotMatch1)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr5.scs");
  loader.loadScsFile(
      ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction action = ctx.ConvertToAction(test_action_node);
  initialize(ctx);
  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinishedUnsuccessfully());
  shutdown(ctx);
}

TEST_F(StandardMessageReplyTest, MessagesAndPhraseClassesDoNotMatch2)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr6.scs");
  loader.loadScsFile(
      ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction action = ctx.ConvertToAction(test_action_node);
  initialize(ctx);

  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinishedUnsuccessfully());

  shutdown(ctx);
}
}  // namespace testStandardMessageReplyAgent
