#include "sc_test.hpp"
#include "sc-builder/src/scs_loader.hpp"
#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
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
  scAgentsCommon::CoreKeynodes::InitGlobal();
  commonModule::Keynodes::InitGlobal();
  commonTest::TestKeynodes::InitGlobal();

  ScAgentInit(true);
  SC_AGENT_REGISTER(NonAtomicActionInterpreterAgent);
  SC_AGENT_REGISTER(QuestionFinishedSuccessfullyTestAgent)
  SC_AGENT_REGISTER(QuestionFinishedTestAgent)
  SC_AGENT_REGISTER(QuestionFinishedUnsuccessfullyTestAgent)
  SC_AGENT_REGISTER(AssignDynamicArgumentTestAgent)
  SC_AGENT_REGISTER(CheckDynamicArgumentTestAgent)
}

void shutdown()
{
  SC_AGENT_UNREGISTER(NonAtomicActionInterpreterAgent);
  SC_AGENT_UNREGISTER(QuestionFinishedSuccessfullyTestAgent)
  SC_AGENT_UNREGISTER(QuestionFinishedTestAgent)
  SC_AGENT_UNREGISTER(QuestionFinishedUnsuccessfullyTestAgent)
  SC_AGENT_UNREGISTER(AssignDynamicArgumentTestAgent)
  SC_AGENT_UNREGISTER(CheckDynamicArgumentTestAgent)
}

ScAddr getFirstAction(ScMemoryContext & context)
{
  ScAddr actionAddr;
  ScTemplate scTemplate;
  scTemplate.Triple(
        context.HelperFindBySystemIdtf("test_nonatomic_action"),
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
        scAgentsCommon::CoreKeynodes::rrel_1);

  ScTemplateSearchResult results;
  context.HelperSearchTemplate(scTemplate, results);
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

  ScAddr test_question_node = context.HelperFindBySystemIdtf("test_question_node");
  EXPECT_TRUE(test_question_node.IsValid());

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        test_question_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished_successfully,
        test_question_node,
        ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished,
        action,
        ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished_successfully, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.Iterator5(
        action,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        context.HelperFindBySystemIdtf("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished_successfully, action, ScType::EdgeAccessConstPosPerm));

  ScTemplate scTemplate;
  scTemplate.TripleWithRelation(
        action,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_dynamic_argument",
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::rrel_1);
  scTemplate.Triple(
        "_dynamic_argument",
        ScType::EdgeAccessVarPosTemp,
        TestKeynodes::test_node >> "_dynamic_argument_value");
  ScTemplateSearchResult results;
  context.HelperSearchTemplate(scTemplate, results);
  EXPECT_TRUE(results.Size() == 1);

  EXPECT_TRUE(results[0]["_dynamic_argument_value"] == context.HelperFindBySystemIdtf("test_node"));

  iterator5.reset();
  shutdown();
}

TEST_F(NonAtomicActionInterpreterTest, checkThenSequence)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "sucsesfullyFinishedSubaction.scs");
  initialize();

  ScAddr test_question_node = context.HelperFindBySystemIdtf("test_question_node");
  EXPECT_TRUE(test_question_node.IsValid());

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        test_question_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished_successfully,
        test_question_node,
        ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished,
        action,
        ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished_successfully, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.Iterator5(
        action,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        context.HelperFindBySystemIdtf("nrel_then"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  iterator5.reset();
  shutdown();
}

TEST_F(NonAtomicActionInterpreterTest, checkElseSequence)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "unsucsesfullyFinishedSubaction.scs");
  initialize();

  ScAddr test_question_node = context.HelperFindBySystemIdtf("test_question_node");
  EXPECT_TRUE(test_question_node.IsValid());

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        test_question_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished_successfully, test_question_node,
        ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.Iterator5(
        action,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        context.HelperFindBySystemIdtf("nrel_else"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  iterator5.reset();
  shutdown();
}

TEST_F(NonAtomicActionInterpreterTest, checkGotoSequence)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "finishedSubaction.scs");
  initialize();

  ScAddr test_question_node = context.HelperFindBySystemIdtf("test_question_node");
  EXPECT_TRUE(test_question_node.IsValid());

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        test_question_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished_successfully, test_question_node,
        ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.Iterator5(
        action,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        context.HelperFindBySystemIdtf("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  iterator5.reset();
  shutdown();
}

TEST_F(NonAtomicActionInterpreterTest, checkArgumentsMatching)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "argumentsMatching.scs");
  initialize();

  ScAddr test_question_node = context.HelperFindBySystemIdtf("test_question_node");
  EXPECT_TRUE(test_question_node.IsValid());

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        test_question_node);

  EXPECT_TRUE(ActionUtils::waitAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished_successfully, test_question_node,
        ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  ScTemplate scTemplate;
  scTemplate.TripleWithRelation(
        action,
        ScType::EdgeAccessVarPosPerm,
        context.HelperFindBySystemIdtf("arg1"),
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::rrel_1);
  scTemplate.TripleWithRelation(
        action,
        ScType::EdgeAccessVarPosPerm,
        context.HelperFindBySystemIdtf("arg3"),
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::rrel_2);
  ScTemplateSearchResult results;
  context.HelperSearchTemplate(scTemplate, results);
  EXPECT_TRUE(results.Size() == 1);

  ScIterator5Ptr iterator5 = context.Iterator5(
        action,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        context.HelperFindBySystemIdtf("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);
  EXPECT_TRUE(context.HelperCheckEdge(
        scAgentsCommon::CoreKeynodes::question_finished, action, ScType::EdgeAccessConstPosPerm));

  scTemplate.TripleWithRelation(
        action,
        ScType::EdgeAccessVarPosPerm,
        context.HelperFindBySystemIdtf("arg2"),
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::rrel_1);
  context.HelperSearchTemplate(scTemplate, results);
  EXPECT_TRUE(results.Size() == 1);

  iterator5.reset();
  shutdown();
}

}//namespace commonTest
