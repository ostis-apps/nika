#include "sc_test.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include <sc-memory/sc_agent.hpp>
#include "agent/NonAtomicActionInterpreterAgent.hpp"
#include "keynodes/Keynodes.hpp"
#include "test/agent/QuestionFinishedSuccessfullyTestAgent.hpp"
#include "test/agent/QuestionFinishedTestAgent.hpp"
#include "test/agent/QuestionFinishedUnsuccessfullyTestAgent.hpp"
#include "test/agent/AssignDynamicArgumentTestAgent.hpp"
#include "test/agent/CheckDynamicArgumentTestAgent.hpp"
#include "test/keynodes/TestKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace commonModule;

namespace commonTest
{
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = COMMON_MODULE_TEST_SRC_PATH "/testStructures/";
const int WAIT_TIME = 5000;

using NonAtomicActionInterpreterTest = ScMemoryTest;

void initialize()
{
  ScKeynodes::InitGlobal();
  commonModule::Keynodes::InitGlobal();
  commonTest::TestKeynodes::InitGlobal();

  ScAgentInit(true);
  //todo(codegen-removal): Use agentContext.SubscribeAgent<NonAtomicActionInterpreterAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(NonAtomicActionInterpreterAgent);
  //todo(codegen-removal): Use agentContext.SubscribeAgent<QuestionFinishedSuccessfullyTestAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(QuestionFinishedSuccessfullyTestAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<QuestionFinishedTestAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(QuestionFinishedTestAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<QuestionFinishedUnsuccessfullyTestAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(QuestionFinishedUnsuccessfullyTestAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<AssignDynamicArgumentTestAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(AssignDynamicArgumentTestAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<CheckDynamicArgumentTestAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_REGISTER(CheckDynamicArgumentTestAgent)
}

void shutdown()
{
  //todo(codegen-removal): Use agentContext.SubscribeAgent<NonAtomicActionInterpreterAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(NonAtomicActionInterpreterAgent);
  //todo(codegen-removal): Use agentContext.SubscribeAgent<QuestionFinishedSuccessfullyTestAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(QuestionFinishedSuccessfullyTestAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<QuestionFinishedTestAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(QuestionFinishedTestAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<QuestionFinishedUnsuccessfullyTestAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(QuestionFinishedUnsuccessfullyTestAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<AssignDynamicArgumentTestAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(AssignDynamicArgumentTestAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<CheckDynamicArgumentTestAgent> or UnsubscribeAgent; to register and unregister agent
SC_AGENT_UNREGISTER(CheckDynamicArgumentTestAgent)
}

ScAddr getFirstAction(ScMemoryContext & context)
{
  ScAddr actionAddr;
  ScTemplate scTemplate;
  scTemplate.Triple(
        context.SearchElementBySystemIdentifier("test_nonatomic_action"),
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_nonAtomicAction");
  scTemplate.TripleWithRelation(
        ScType::NodeVar >> "_tuple",
        ScType::EdgeDCommonVar,
        "_nonAtomicAction",
        ScType::EdgeAccessVarPosPerm,
        Keynodes::nrel_decomposition_of_action);
  scTemplate.TripleWithRelation(
        "_tuple",
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_firstAction",
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_1);

  ScTemplateSearchResult results;
  context.SearchByTemplate(scTemplate, results);
  if (results.Size() == 1)
  {
    actionAddr = results[0]["_firstAction"];
  }
  return actionAddr;
}

TEST_F(NonAtomicActionInterpreterTest, checkDynamicArguments)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "dynamicArguments.scs");
  initialize();

  ScAddr test_question_node = context.SearchElementBySystemIdentifier("test_question_node");
  EXPECT_TRUE(test_question_node.IsValid());

  context.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_question_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished_successfully,
        test_question_node,
        ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished,
        action,
        ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished_successfully, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.CreateIterator5(
        action,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        context.SearchElementBySystemIdentifier("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished_successfully, action, ScType::EdgeAccessConstPosPerm));

  ScTemplate scTemplate;
  scTemplate.TripleWithRelation(
        action,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_dynamic_argument",
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_1);
  scTemplate.Triple(
        "_dynamic_argument",
        ScType::EdgeAccessVarPosTemp,
        TestKeynodes::test_node >> "_dynamic_argument_value");
  ScTemplateSearchResult results;
  context.SearchByTemplate(scTemplate, results);
  EXPECT_TRUE(results.Size() == 1);

  EXPECT_TRUE(results[0]["_dynamic_argument_value"] == context.SearchElementBySystemIdentifier("test_node"));

  iterator5.reset();
  shutdown();
}

TEST_F(NonAtomicActionInterpreterTest, checkThenSequence)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "sucsesfullyFinishedSubaction.scs");
  initialize();

  ScAddr test_question_node = context.SearchElementBySystemIdentifier("test_question_node");
  EXPECT_TRUE(test_question_node.IsValid());

  context.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_question_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished_successfully,
        test_question_node,
        ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished,
        action,
        ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished_successfully, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.CreateIterator5(
        action,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        context.SearchElementBySystemIdentifier("nrel_then"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  iterator5.reset();
  shutdown();
}

TEST_F(NonAtomicActionInterpreterTest, checkElseSequence)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "unsucsesfullyFinishedSubaction.scs");
  initialize();

  ScAddr test_question_node = context.SearchElementBySystemIdentifier("test_question_node");
  EXPECT_TRUE(test_question_node.IsValid());

  context.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_question_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished_successfully, test_question_node,
        ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished_unsuccessfully, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.CreateIterator5(
        action,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        context.SearchElementBySystemIdentifier("nrel_else"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  iterator5.reset();
  shutdown();
}

TEST_F(NonAtomicActionInterpreterTest, checkGotoSequence)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "finishedSubaction.scs");
  initialize();

  ScAddr test_question_node = context.SearchElementBySystemIdentifier("test_question_node");
  EXPECT_TRUE(test_question_node.IsValid());

  context.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_question_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished_successfully, test_question_node,
        ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.CreateIterator5(
        action,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        context.SearchElementBySystemIdentifier("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  iterator5.reset();
  shutdown();
}

TEST_F(NonAtomicActionInterpreterTest, checkArgumentsMatching)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "argumentsMatching.scs");
  initialize();

  ScAddr test_question_node = context.SearchElementBySystemIdentifier("test_question_node");
  EXPECT_TRUE(test_question_node.IsValid());

  context.GenerateConnector(
        ScType::EdgeAccessConstPosPerm,
        ScKeynodes::question_initiated,
        test_question_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished_successfully, test_question_node,
        ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  ScTemplate scTemplate;
  scTemplate.TripleWithRelation(
        action,
        ScType::EdgeAccessVarPosPerm,
        context.SearchElementBySystemIdentifier("arg1"),
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_1);
  scTemplate.TripleWithRelation(
        action,
        ScType::EdgeAccessVarPosPerm,
        context.SearchElementBySystemIdentifier("arg3"),
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_2);
  ScTemplateSearchResult results;
  context.SearchByTemplate(scTemplate, results);
  EXPECT_TRUE(results.Size() == 1);

  ScIterator5Ptr iterator5 = context.CreateIterator5(
        action,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        context.SearchElementBySystemIdentifier("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);
  EXPECT_TRUE(context.CheckConnector(
        ScKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  scTemplate.TripleWithRelation(
        action,
        ScType::EdgeAccessVarPosPerm,
        context.SearchElementBySystemIdentifier("arg2"),
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_1);
  context.SearchByTemplate(scTemplate, results);
  EXPECT_TRUE(results.Size() == 1);

  iterator5.reset();
  shutdown();
}

}//namespace commonTest
