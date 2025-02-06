#include <sc-memory/sc_agent.hpp>
#include <sc-memory/test/sc_test.hpp>

#include "agent/PhraseGenerationAgent.hpp"
#include "agent/StandardMessageReplyAgent.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "searcher/TokenDomainSearcher.hpp"
#include <inference/direct_inference_agent.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-builder/scs_loader.hpp>

using namespace dialogControlModule;

namespace testStandardMessageReplyAgent
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = "../tests-structures/standard-message-reply/";
std::string const ACTION_DIR_PATH = "actions/";
std::string const LR_DIR_PATH = "logic-rules/";
std::string const MES_DIR_PATH = "messages/";
std::string const PHRASE_DIR_PATH = "phrases/";
int const WAIT_TIME = 20000;

using StandardMessageReplyTest = ScMemoryTest;

void initialize(ScAgentContext & context)
{
  context.SubscribeAgent<inference::DirectInferenceAgent>();
  context.SubscribeAgent<PhraseGenerationAgent>();
  context.SubscribeAgent<StandardMessageReplyAgent>();
}

void shutdown(ScAgentContext & context)
{
  context.UnsubscribeAgent<inference::DirectInferenceAgent>();
  context.UnsubscribeAgent<PhraseGenerationAgent>();
  context.UnsubscribeAgent<StandardMessageReplyAgent>();
}

TEST_F(StandardMessageReplyTest, ProcessingAtomicMessageIsSuccessful)
{
  static std::string const messageText = "I'm sorry, Mister Ivanov.";
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr4.scs");
  loader.loadScsFile(
      context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");

  ScAction testAction = context.ConvertToAction(testActionNode);

  initialize(context);

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));

  ScAddr messageNode = context.SearchElementBySystemIdentifier("test_message");
  EXPECT_TRUE(messageNode.IsValid());

  ScAddr replyMessageNode =
      utils::IteratorUtils::getAnyByOutRelation(&context, messageNode, MessageKeynodes::nrel_reply);
  EXPECT_TRUE(replyMessageNode.IsValid());

  TokenDomainSearcher searcher(&context);
  ScAddr messageLink = searcher.getMessageText(replyMessageNode);
  std::string linkContent;
  context.GetLinkContent(messageLink, linkContent);
  EXPECT_EQ(linkContent, messageText);

  shutdown(context);
}

TEST_F(StandardMessageReplyTest, ProcessingNonatomicMessageIsSuccessful)
{
  static std::string const messageText = "I'm sorry, Mister Ivanov. How did this happen, Ivan?";
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(
      context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase2.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");

  ScAction action = context.ConvertToAction(test_action_node);

  initialize(context);
  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  shutdown(context);

  ScAddr messageNode = context.SearchElementBySystemIdentifier("test_message");
  EXPECT_TRUE(messageNode.IsValid());

  ScAddr replyMessageNode =
      utils::IteratorUtils::getAnyByOutRelation(&context, messageNode, MessageKeynodes::nrel_reply);
  EXPECT_TRUE(replyMessageNode.IsValid());

  TokenDomainSearcher searcher(&context);
  ScAddr messageLink = searcher.getMessageText(replyMessageNode);
  std::string linkContent;
  context.GetLinkContent(messageLink, linkContent);
  EXPECT_EQ(linkContent, messageText);
}

TEST_F(StandardMessageReplyTest, ActionDoesntHaveAMessageNode)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "incorrectAction.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<StandardMessageReplyAgent>();
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedWithError());
  context.UnsubscribeAgent<StandardMessageReplyAgent>();
}

TEST_F(StandardMessageReplyTest, SystemDoesNotHaveTemplateForMessage)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(
      context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message1.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<StandardMessageReplyAgent>();
  context.SubscribeAgent<inference::DirectInferenceAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedWithError());

  context.UnsubscribeAgent<inference::DirectInferenceAgent>();
  context.UnsubscribeAgent<StandardMessageReplyAgent>();
}

TEST_F(StandardMessageReplyTest, MessagesLanguageIsNotFound)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(
      context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message2.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<StandardMessageReplyAgent>();
  context.SubscribeAgent<inference::DirectInferenceAgent>();
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());
  context.UnsubscribeAgent<inference::DirectInferenceAgent>();
  context.UnsubscribeAgent<StandardMessageReplyAgent>();
}

TEST_F(StandardMessageReplyTest, FirstMessageIsNotFound)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr2.scs");
  loader.loadScsFile(
      context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<StandardMessageReplyAgent>();
  context.SubscribeAgent<inference::DirectInferenceAgent>();
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());
  context.UnsubscribeAgent<StandardMessageReplyAgent>();
  context.UnsubscribeAgent<inference::DirectInferenceAgent>();
}

TEST_F(StandardMessageReplyTest, FirstPhraseClassIsNotFound)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr3.scs");
  loader.loadScsFile(
      context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<StandardMessageReplyAgent>();
  context.SubscribeAgent<inference::DirectInferenceAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());

  context.UnsubscribeAgent<StandardMessageReplyAgent>();
  context.UnsubscribeAgent<inference::DirectInferenceAgent>();
}

TEST_F(StandardMessageReplyTest, PhraseForMessageIsNotFound)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(
      context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<StandardMessageReplyAgent>();
  context.SubscribeAgent<inference::DirectInferenceAgent>();
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());
  context.UnsubscribeAgent<StandardMessageReplyAgent>();
  context.UnsubscribeAgent<inference::DirectInferenceAgent>();
}

TEST_F(StandardMessageReplyTest, LinkByPhraseIsNotGenerated)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr1.scs");
  loader.loadScsFile(
      context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message3.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);
  initialize(context);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());
  shutdown(context);
}

TEST_F(StandardMessageReplyTest, MessagesAndPhraseClassesDoNotMatch1)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr5.scs");
  loader.loadScsFile(
      context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);
  initialize(context);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());
  shutdown(context);
}

TEST_F(StandardMessageReplyTest, MessagesAndPhraseClassesDoNotMatch2)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + ACTION_DIR_PATH + "correctAction.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "lr6.scs");
  loader.loadScsFile(
      context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "concept_answer_on_standard_message_rule_class_by_priority.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + LR_DIR_PATH + "reply_target.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + MES_DIR_PATH + "message4.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + PHRASE_DIR_PATH + "phrase1.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);
  initialize(context);

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());

  shutdown(context);
}
}  // namespace testStandardMessageReplyAgent
