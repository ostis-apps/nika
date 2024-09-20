#include "agent/PhraseGenerationAgent.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include "handler/LinkHandler.cpp"
#include "keynodes/DialogKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "sc_test.hpp"
#include <sc-memory/sc_agent.hpp>
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
  ScKeynodes::InitGlobal();
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
        ScKeynodes::nrel_answer
  );
  EXPECT_TRUE(ctx.SearchByTemplate(scTemplate, result));

  ScAddr link = result[0][LINK_ALIAS];

  EXPECT_EQ(utils::CommonUtils::getLinkContent(&ctx, link), resultPhrase);
}

TEST_F(PhraseGenerationTest, ActionDoesNotHaveALinkTemplate)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "actionDoesntHaveLinkTemplateTest.scs");
  ScAddr test_action_node = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_action_node);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, TestIsCorrect)
{
  string resultPhrase = "Sorry, Guilderoy Lockhart.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "correctTest.scs");
  ScAddr testQuestionNode = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, TestIsCorrect2)
{
  string resultPhrase = "Mariya Ivanova is the mother of Ivan Ivanov.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "correctTest2.scs");
  ScAddr testQuestionNode = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, TestIsCorrect3)
{
  string resultPhrase = "Ivan Ivanov is the brother of Pavel Ivanov.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "correctTest3.scs");
  ScAddr testQuestionNode = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, TestTemplateHasNoVariables)
{
  string resultPhrase = "Sorry.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "textTemplateDoesntHaveVariables.scs");
  ScAddr testQuestionNode = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, AgentDoesNotGenerateAPhrase)
{
  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "agentDoesntGenerateAPhraseTest.scs");
  ScAddr testQuestionNode = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, GeneratedLinkDoesNotHaveALanguageNode)
{
  ScMemoryContext & ctx = *m_ctx;
  LanguageSearcher searcher(&ctx);

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "generatedLinkDoesntHaveALanguageNodeTest.scs");
  ScAddr testQuestionNode = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  ScTemplate scTemplate;
  ScTemplateSearchResult result;
  const std::string LINK_ALIAS = "_link";
  scTemplate.TripleWithRelation(
        testQuestionNode,
        ScType::EdgeDCommonVar,
        ScType::LinkVar >> LINK_ALIAS,
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::nrel_answer
  );
  EXPECT_TRUE(ctx.SearchByTemplate(scTemplate, result));

  ScAddr link = result[0][LINK_ALIAS];
  ScAddr langNode = searcher.getLanguage(link);

  EXPECT_FALSE(langNode.IsValid());

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, QuestionDoesNotHaveAnyParameters)
{
  string resultPhrase = "Sorry, Guilderoy Lockhart.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "questionDoesntHaveParametersTest.scs");
  ScAddr testQuestionNode = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_unsuccessfully).
              Wait(WAIT_TIME));

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}

TEST_F(PhraseGenerationTest, ParameterNodeIsEmpty)
{
  string resultPhrase = "Sorry, Guilderoy Lockhart.";

  ScMemoryContext & ctx = *m_ctx;

  loader.loadScsFile(ctx, TEST_FILES_DIR_PATH + "parametersNodeAreEmptyTest.scs");
  ScAddr testQuestionNode = ctx.SearchElementBySystemIdentifier("test_question_node");

  ScAgentInit(true);
  initializeClasses();

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(dialogControlModule::PhraseGenerationAgent)

  ctx.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        testQuestionNode);

  EXPECT_TRUE(
        ScWaitEvent<ScEventAfterGenerateOutgoingArc>(
              ctx,
              ScKeynodes::question_finished_successfully).
              Wait(WAIT_TIME));

  checkLinkContent(ctx, testQuestionNode, resultPhrase);

  //todo(codegen-removal): Use agentContext.SubscribeAgent<dialogControlModule::PhraseGenerationAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(dialogControlModule::PhraseGenerationAgent)
}
}
