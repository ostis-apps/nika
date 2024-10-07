#include <sc-memory/sc_agent.hpp>

#include "sc-builder/src/scs_loader.hpp"
#include "sc_test.hpp"

#include "agent/PhraseGenerationAgent.hpp"
#include "handler/LinkHandler.cpp"
#include "searcher/LanguageSearcher.hpp"

using namespace dialogControlModule;
using namespace commonModule;

namespace testPhraseGenerationAgent
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = DIALOG_CONTROL_MODULE_TEST_SRC_PATH "/testsStructures/phraseGeneration/";
int const WAIT_TIME = 5000;

using PhraseGenerationTest = ScMemoryTest;

void checkLinkContent(ScAgentContext & context, ScAddr action, std::string const & resultPhrase)
{
  ScTemplate scTemplate;
  ScTemplateSearchResult result;
  std::string const LINK_ALIAS = "_link";
  scTemplate.Quintuple(
      action,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> LINK_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::nrel_result);
  EXPECT_TRUE(context.SearchByTemplate(scTemplate, result));

  ScAddr link = result[0][LINK_ALIAS];
  std::string linkContent;
  context.GetLinkContent(link, linkContent);
  EXPECT_EQ(linkContent, resultPhrase);
}

TEST_F(PhraseGenerationTest, ActionDoesNotHaveALinkTemplate)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "actionDoesntHaveLinkTemplateTest.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedWithError());

  context.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, TestIsCorrect)
{
  std::string resultPhrase = "Sorry, Guilderoy Lockhart.";

  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "correctTest.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  checkLinkContent(context, test_action_node, resultPhrase);
  context.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, TestIsCorrect2)
{
  std::string resultPhrase = "Mariya Ivanova is the mother of Ivan Ivanov.";

  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "correctTest2.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  checkLinkContent(context, test_action_node, resultPhrase);
  context.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, TestIsCorrect3)
{
  std::string resultPhrase = "Ivan Ivanov is the brother of Pavel Ivanov.";

  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "correctTest3.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  checkLinkContent(context, test_action_node, resultPhrase);

  context.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, TestTemplateHasNoVariables)
{
  std::string resultPhrase = "Sorry.";

  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "textTemplateDoesntHaveVariables.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  checkLinkContent(context, test_action_node, resultPhrase);

  context.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, AgentDoesNotGenerateAPhrase)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "agentDoesntGenerateAPhraseTest.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());
  context.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, GeneratedLinkDoesNotHaveALanguageNode)
{
  ScAgentContext & context = *m_ctx;
  LanguageSearcher searcher(&context);

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "generatedLinkDoesntHaveALanguageNodeTest.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScTemplate scTemplate;
  ScTemplateSearchResult result;
  std::string const LINK_ALIAS = "_link";
  scTemplate.Quintuple(
      test_action_node,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> LINK_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::nrel_result);
  EXPECT_TRUE(context.SearchByTemplate(scTemplate, result));

  ScAddr link = result[0][LINK_ALIAS];
  ScAddr langNode = searcher.getLanguage(link);

  EXPECT_FALSE(langNode.IsValid());

  context.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, actionDoesNotHaveAnyParameters)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "actionDoesntHaveParametersTest.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedWithError());

  context.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}

TEST_F(PhraseGenerationTest, ParameterNodeIsEmpty)
{
  ScAgentContext & context = *m_ctx;
  std::string const resultPhrase = "Sorry, Guilderoy Lockhart.";

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "parametersNodeAreEmptyTest.scs");
  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<dialogControlModule::PhraseGenerationAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  checkLinkContent(context, test_action_node, resultPhrase);
  context.UnsubscribeAgent<dialogControlModule::PhraseGenerationAgent>();
}
}  // namespace testPhraseGenerationAgent
