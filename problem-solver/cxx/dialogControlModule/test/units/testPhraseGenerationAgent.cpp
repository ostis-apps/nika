#include "agent/PhraseGenerationAgent.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include "handler/LinkHandler.cpp"
#include "keynodes/DialogKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "sc_test.hpp"
#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-memory/sc_wait.hpp"
#include "searcher/LanguageSearcher.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"

using namespace dialogControlModule;
using namespace commonModule;

namespace testPhraseGenerationAgent
{
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = DIALOG_CONTROL_MODULE_TEST_SRC_PATH "/TestsStructures/PhraseGeneration/";
const int WAIT_TIME = 5000;

using PhraseGenerationTest = ScMemoryTest;

void initializeClasses()
{
  scAgentsCommon::CoreKeynodes::InitGlobal();
  DialogKeynodes::InitGlobal();
  MessageKeynodes::InitGlobal();
  Keynodes::InitGlobal();
}

void checkLinkContent(ScMemoryContext & ctx, ScAddr action, const string& resultPhrase)
{
  ScTemplate scTemplate;
  ScTemplateSearchResult result;
  const std::string LINK_ALIAS = "_link";
  scTemplate.TripleWithRelation(
        action,
        ScType::EdgeDCommonVar,
        ScType::LinkVar >> LINK_ALIAS,
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::nrel_answer
  );
  EXPECT_TRUE(ctx.HelperSearchTemplate(scTemplate, result));

  ScAddr link = result[0][LINK_ALIAS];

  EXPECT_EQ(utils::CommonUtils::getLinkContent(&ctx, link), resultPhrase);
}

TEST_F(PhraseGenerationTest, ActionDoesNotHaveALinkTemplate)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "actionDoesntHaveLinkTemplateTest.scs");
  ScAddr test_action_node = ctx.HelperFindBySystemIdtf("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAddOutputEdge>(
              ctx,
              scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, TestIsCorrect)
{
  string resultPhrase = "Sorry, Guilderoy Lockhart.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "correctTest.scs");
  ScAddr testQuestionNode = ctx.HelperFindBySystemIdtf("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAddOutputEdge>(
              ctx,
              scAgentsCommon::CoreKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, TestIsCorrect2)
{
  string resultPhrase = "Mariya Ivanova is the mother of Ivan Ivanov.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "correctTest2.scs");
  ScAddr testQuestionNode = ctx.HelperFindBySystemIdtf("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAddOutputEdge>(
              ctx,
              scAgentsCommon::CoreKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, TestIsCorrect3)
{
  string resultPhrase = "Ivan Ivanov is the brother of Pavel Ivanov.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "correctTest3.scs");
  ScAddr testQuestionNode = ctx.HelperFindBySystemIdtf("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAddOutputEdge>(
              ctx,
              scAgentsCommon::CoreKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, TestTemplateHasNoVariables)
{
  string resultPhrase = "Sorry.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "textTemplateDoesntHaveVariables.scs");
  ScAddr testQuestionNode = ctx.HelperFindBySystemIdtf("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAddOutputEdge>(
              ctx,
              scAgentsCommon::CoreKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, AgentDoesNotGenerateAPhrase)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "agentDoesntGenerateAPhraseTest.scs");
  ScAddr testQuestionNode = ctx.HelperFindBySystemIdtf("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAddOutputEdge>(
              ctx,
              scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, GeneratedLinkDoesNotHaveALanguageNode)
{
  ScMemoryContext & ctx = *m_ctx;
  LanguageSearcher searcher(&ctx);

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generatedLinkDoesntHaveALanguageNodeTest.scs");
  ScAddr testQuestionNode = ctx.HelperFindBySystemIdtf("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAddOutputEdge>(
              ctx,
              scAgentsCommon::CoreKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  ScTemplate scTemplate;
  ScTemplateSearchResult result;
  const std::string LINK_ALIAS = "_link";
  scTemplate.TripleWithRelation(
        testQuestionNode,
        ScType::EdgeDCommonVar,
        ScType::LinkVar >> LINK_ALIAS,
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::nrel_answer
  );
  EXPECT_TRUE(ctx.HelperSearchTemplate(scTemplate, result));

  ScAddr link = result[0][LINK_ALIAS];
  ScAddr langNode = searcher.getLanguage(link);

  EXPECT_FALSE(langNode.IsValid());

  SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, QuestionDoesNotHaveAnyParameters)
{
  string resultPhrase = "Sorry, Guilderoy Lockhart.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "questionDoesntHaveParametersTest.scs");
  ScAddr testQuestionNode = ctx.HelperFindBySystemIdtf("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAddOutputEdge>(
              ctx,
              scAgentsCommon::CoreKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, ParameterNodeIsEmpty)
{
  string resultPhrase = "Sorry, Guilderoy Lockhart.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "parametersNodeAreEmptyTest.scs");
  ScAddr testQuestionNode = ctx.HelperFindBySystemIdtf("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAddOutputEdge>(
              ctx,
              scAgentsCommon::CoreKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}
}
