#include <sc-memory/sc_agent.hpp>

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include "sc_test.hpp"

#include "agent/PhraseGenerationAgent.hpp"
#include "handler/LinkHandler.cpp"
#include "keynodes/DialogKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "searcher/LanguageSearcher.hpp"

using namespace dialogControlModule;
using namespace commonModule;

namespace testPhraseGenerationAgent
{
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = DIALOG_CONTROL_MODULE_TEST_SRC_PATH "/TestsStructures/PhraseGeneration/";
const int WAIT_TIME = 5000;

using PhraseGenerationTest = ScMemoryTest;

void checkLinkContent(ScAgentContext & ctx, ScAddr action, const std::string & resultPhrase)
{
  ScTemplate scTemplate;
  ScTemplateSearchResult result;
  const std::string LINK_ALIAS = "_link";
  scTemplate.Quintuple(
      action,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> LINK_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::nrel_result);
  EXPECT_TRUE(ctx.SearchByTemplate(scTemplate, result));

  ScAddr link = result[0][LINK_ALIAS];
  std::string linkContent;
  ctx.GetLinkContent(link, linkContent);
  EXPECT_EQ(linkContent, resultPhrase);
}

TEST_F(PhraseGenerationTest, ActionDoesNotHaveALinkTemplate)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "actionDoesntHaveLinkTemplateTest.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_FALSE(testAction.IsFinishedUnsuccessfully());

  ctx.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, TestIsCorrect)
{
  std::string resultPhrase = "Sorry, Guilderoy Lockhart.";

  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "correctTest.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  checkLinkContent(ctx, test_action_node, resultPhrase);
  ctx.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, TestIsCorrect2)
{
  std::string resultPhrase = "Mariya Ivanova is the mother of Ivan Ivanov.";

  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "correctTest2.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  checkLinkContent(ctx, test_action_node, resultPhrase);
  ctx.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, TestIsCorrect3)
{
  std::string resultPhrase = "Ivan Ivanov is the brother of Pavel Ivanov.";

  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "correctTest3.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  checkLinkContent(ctx, test_action_node, resultPhrase);

  ctx.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, TestTemplateHasNoVariables)
{
  std::string resultPhrase = "Sorry.";

  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "textTemplateDoesntHaveVariables.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  checkLinkContent(ctx, test_action_node, resultPhrase);

  ctx.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, AgentDoesNotGenerateAPhrase)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "agentDoesntGenerateAPhraseTest.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());
  ctx.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, GeneratedLinkDoesNotHaveALanguageNode)
{
  ScAgentContext & ctx = *m_ctx;
  LanguageSearcher searcher(&ctx);

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generatedLinkDoesntHaveALanguageNodeTest.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScTemplate scTemplate;
  ScTemplateSearchResult result;
  const std::string LINK_ALIAS = "_link";
  scTemplate.Quintuple(
      test_action_node,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> LINK_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::nrel_result);
  EXPECT_TRUE(ctx.SearchByTemplate(scTemplate, result));

  ScAddr link = result[0][LINK_ALIAS];
  ScAddr langNode = searcher.getLanguage(link);

  EXPECT_FALSE(langNode.IsValid());

  ctx.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, actionDoesNotHaveAnyParameters)
{
  ScAgentContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "actionDoesntHaveParametersTest.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedWithError());

  ctx.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, ParameterNodeIsEmpty)
{
  ScAgentContext & ctx = *m_ctx;
  std::string const resultPhrase = "Sorry, Guilderoy Lockhart.";

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "parametersNodeAreEmptyTest.scs");
  ScAddr const & test_action_node = ctx.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = ctx.ConvertToAction(test_action_node);

  ctx.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  checkLinkContent(ctx, test_action_node, resultPhrase);
  ctx.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}
}  // namespace testPhraseGenerationAgent
