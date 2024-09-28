#include <sc-memory/sc_agent.hpp>

#include "sc-builder/src/scs_loader.hpp"
#include "sc_test.hpp"

#include "agent/NonAtomicActionInterpreterAgent.hpp"
#include "keynodes/Keynodes.hpp"
#include "test/agent/AssignDynamicArgumentTestAgent.hpp"
#include "test/agent/CheckDynamicArgumentTestAgent.hpp"
#include "test/agent/QuestionFinishedSuccessfullyTestAgent.hpp"
#include "test/agent/QuestionFinishedTestAgent.hpp"
#include "test/agent/QuestionFinishedUnsuccessfullyTestAgent.hpp"
#include "test/keynodes/TestKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace commonModule;

namespace commonTest
{
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = COMMON_MODULE_TEST_SRC_PATH "/testStructures/";
const int WAIT_TIME = 5000;

using NonAtomicActionInterpreterTest = ScMemoryTest;

void initialize(ScAgentContext & context)
{
  context.SubscribeAgent<NonAtomicActionInterpreterAgent>();
  context.SubscribeAgent<ActionFinishedSuccessfullyTestAgent>();
  context.SubscribeAgent<ActionFinishedTestAgent>();
  context.SubscribeAgent<ActionFinishedUnsuccessfullyTestAgent>();
  context.SubscribeAgent<AssignDynamicArgumentTestAgent>();
  context.SubscribeAgent<CheckDynamicArgumentTestAgent>();
}

void shutdown(ScAgentContext & context)
{
  context.UnsubscribeAgent<NonAtomicActionInterpreterAgent>();
  context.UnsubscribeAgent<ActionFinishedSuccessfullyTestAgent>();
  context.UnsubscribeAgent<ActionFinishedTestAgent>();
  context.UnsubscribeAgent<ActionFinishedUnsuccessfullyTestAgent>();
  context.UnsubscribeAgent<AssignDynamicArgumentTestAgent>();
  context.UnsubscribeAgent<CheckDynamicArgumentTestAgent>();
}

ScAddr getFirstAction(ScAgentContext & context)
{
  ScAddr actionAddr;
  ScTemplate scTemplate;
  scTemplate.Triple(
      context.SearchElementBySystemIdentifier("test_nonatomic_action"),
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_nonAtomicAction");
  scTemplate.Quintuple(
      ScType::NodeVar >> "_tuple",
      ScType::EdgeDCommonVar,
      "_nonAtomicAction",
      ScType::EdgeAccessVarPosPerm,
      Keynodes::nrel_decomposition_of_action);
  scTemplate.Quintuple(
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
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "dynamicArguments.scs");

  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");

  EXPECT_TRUE(test_action_node.IsValid());
  initialize(context);

  // EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  ScAction actionNode = context.ConvertToAction(test_action_node);
  EXPECT_TRUE(actionNode.InitiateAndWait(WAIT_TIME));
  shutdown(context);
  EXPECT_TRUE(context.CheckConnector(
      ScKeynodes::action_finished_successfully, test_action_node, ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished, action, ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished_successfully, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.CreateIterator5(
      action,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      context.SearchElementBySystemIdentifier("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished, action, ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished_successfully, action, ScType::EdgeAccessConstPosPerm));

  ScTemplate scTemplate;
  scTemplate.Quintuple(
      action,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_dynamic_argument",
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::rrel_1);
  scTemplate.Triple(
      "_dynamic_argument", ScType::EdgeAccessVarPosTemp, TestKeynodes::test_node >> "_dynamic_argument_value");
  ScTemplateSearchResult results;
  context.SearchByTemplate(scTemplate, results);
  EXPECT_TRUE(results.Size() == 1);

  EXPECT_TRUE(results[0]["_dynamic_argument_value"] == context.SearchElementBySystemIdentifier("test_node"));

  iterator5.reset();
}

TEST_F(NonAtomicActionInterpreterTest, checkThenSequence)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "sucsesfullyFinishedSubaction.scs");

  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(test_action_node.IsValid());

  initialize(context);
  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  shutdown(context);
  EXPECT_TRUE(context.CheckConnector(
      ScKeynodes::action_finished_successfully, test_action_node, ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished, action, ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished_successfully, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.CreateIterator5(
      action,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      context.SearchElementBySystemIdentifier("nrel_then"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished, action, ScType::EdgeAccessConstPosPerm));

  iterator5.reset();
}

TEST_F(NonAtomicActionInterpreterTest, checkElseSequence)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "unsucsesfullyFinishedSubaction.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");

  EXPECT_TRUE(test_action_node.IsValid());

  initialize(context);
  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  shutdown(context);
  EXPECT_TRUE(context.CheckConnector(
      ScKeynodes::action_finished_successfully, test_action_node, ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished, action, ScType::EdgeAccessConstPosPerm));
  EXPECT_TRUE(
      context.CheckConnector(ScKeynodes::action_finished_unsuccessfully, action, ScType::EdgeAccessConstPosPerm));

  ScIterator5Ptr iterator5 = context.CreateIterator5(
      action,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      context.SearchElementBySystemIdentifier("nrel_else"));
  EXPECT_TRUE(iterator5->Next());
  action = iterator5->Get(2);

  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished, action, ScType::EdgeAccessConstPosPerm));

  iterator5.reset();
}

TEST_F(NonAtomicActionInterpreterTest, checkGotoSequence)
{
  // TODO(Semyon Protas): After using the scp-machine instead of the non-atomic action interpreter, check for execution
  // of goto together with else and true
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "finishedSubaction.scs");

  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction test_action = context.ConvertToAction(test_action_node);
  EXPECT_TRUE(test_action_node.IsValid());

  initialize(context);
  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  shutdown(context);
  EXPECT_TRUE(test_action.IsFinishedSuccessfully());

  ScAction action = context.ConvertToAction(getFirstAction(context));
  EXPECT_TRUE(action.IsFinished());

  ScIterator5Ptr iterator5 = context.CreateIterator5(
      action,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      context.SearchElementBySystemIdentifier("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  ScAddr next_action = iterator5->Get(2);

  EXPECT_TRUE(context.ConvertToAction(next_action).IsFinished());

  iterator5.reset();
}

TEST_F(NonAtomicActionInterpreterTest, checkArgumentsMatching)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "argumentsMatching.scs");
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(test_action_node.IsValid());

  initialize(context);
  EXPECT_TRUE(ActionUtils::waitAction(&context, test_action_node, WAIT_TIME));
  shutdown(context);

  EXPECT_TRUE(context.CheckConnector(
      ScKeynodes::action_finished_successfully, test_action_node, ScType::EdgeAccessConstPosPerm));

  ScAddr action = getFirstAction(context);
  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished, action, ScType::EdgeAccessConstPosPerm));

  ScTemplate scTemplate;
  scTemplate.Quintuple(
      action,
      ScType::EdgeAccessVarPosPerm,
      context.SearchElementBySystemIdentifier("arg1"),
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::rrel_1);
  scTemplate.Quintuple(
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
  EXPECT_TRUE(context.CheckConnector(ScKeynodes::action_finished, action, ScType::EdgeAccessConstPosPerm));

  scTemplate.Quintuple(
      action,
      ScType::EdgeAccessVarPosPerm,
      context.SearchElementBySystemIdentifier("arg2"),
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::rrel_1);
  context.SearchByTemplate(scTemplate, results);
  EXPECT_TRUE(results.Size() == 1);

  iterator5.reset();
}

}  // namespace commonTest
