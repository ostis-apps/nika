#include <sc-memory/sc_agent.hpp>
#include <sc-memory/test/sc_test.hpp>

#include "agent/AssignDynamicArgumentTestAgent.hpp"
#include "agent/CheckDynamicArgumentTestAgent.hpp"
#include "agent/NonAtomicActionInterpreterAgent.hpp"
#include "agent/QuestionFinishedSuccessfullyTestAgent.hpp"
#include "agent/QuestionFinishedTestAgent.hpp"
#include "agent/QuestionFinishedUnsuccessfullyTestAgent.hpp"
#include "keynodes/TestKeynodes.hpp"
#include <common/keynodes/Keynodes.hpp>
#include <sc-builder/scs_loader.hpp>

using namespace nonAtomicActionInterpreterModule;

namespace commonTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = "../test-structures/";
int const WAIT_TIME = 5000;

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

ScAction getFirstAction(ScAgentContext & context)
{
  ScAddr actionAddr;
  ScTemplate scTemplate;
  scTemplate.Triple(
      context.SearchElementBySystemIdentifier("test_nonatomic_action"),
      ScType::VarPermPosArc,
      ScType::VarNode >> "_nonAtomicAction");
  scTemplate.Quintuple(
      ScType::VarNode >> "_tuple",
      ScType::VarCommonArc,
      "_nonAtomicAction",
      ScType::VarPermPosArc,
      commonModule::Keynodes::nrel_decomposition_of_action);
  scTemplate.Quintuple(
      "_tuple", ScType::VarPermPosArc, ScType::VarNode >> "_firstAction", ScType::VarPermPosArc, ScKeynodes::rrel_1);

  ScTemplateSearchResult results;
  context.SearchByTemplate(scTemplate, results);
  if (results.Size() == 1)
  {
    actionAddr = results[0]["_firstAction"];
  }
  return context.ConvertToAction(actionAddr);
}

TEST_F(NonAtomicActionInterpreterTest, checkDynamicArguments)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "dynamicArguments.scs");

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");

  EXPECT_TRUE(testActionNode.IsValid());
  initialize(context);

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  shutdown(context);
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScAction action = getFirstAction(context);
  EXPECT_TRUE(action.IsFinishedSuccessfully());

  ScIterator5Ptr iterator5 = context.CreateIterator5(
      action,
      ScType::ConstCommonArc,
      ScType::ConstNode,
      ScType::ConstPermPosArc,
      context.SearchElementBySystemIdentifier("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  action = context.ConvertToAction(iterator5->Get(2));

  EXPECT_TRUE(action.IsFinishedSuccessfully());

  ScTemplate scTemplate;
  scTemplate.Quintuple(
      action, ScType::VarPermPosArc, ScType::VarNode >> "_dynamic_argument", ScType::VarPermPosArc, ScKeynodes::rrel_1);
  scTemplate.Triple("_dynamic_argument", ScType::VarTempPosArc, TestKeynodes::test_node >> "_dynamic_argument_value");
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

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  initialize(context);
  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  shutdown(context);
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScAction action = getFirstAction(context);
  EXPECT_TRUE(action.IsFinishedSuccessfully());

  ScIterator5Ptr iterator5 = context.CreateIterator5(
      action,
      ScType::ConstCommonArc,
      ScType::ConstNode,
      ScType::ConstPermPosArc,
      context.SearchElementBySystemIdentifier("nrel_then"));
  EXPECT_TRUE(iterator5->Next());
  action = context.ConvertToAction(iterator5->Get(2));

  EXPECT_TRUE(action.IsFinishedSuccessfully());

  iterator5.reset();
}

TEST_F(NonAtomicActionInterpreterTest, checkElseSequence)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "unsucsesfullyFinishedSubaction.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");

  EXPECT_TRUE(testActionNode.IsValid());

  initialize(context);
  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  shutdown(context);
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScAction action = getFirstAction(context);
  EXPECT_TRUE(action.IsFinishedUnsuccessfully());

  ScIterator5Ptr iterator5 = context.CreateIterator5(
      action,
      ScType::ConstCommonArc,
      ScType::ConstNode,
      ScType::ConstPermPosArc,
      context.SearchElementBySystemIdentifier("nrel_else"));
  EXPECT_TRUE(iterator5->Next());
  action = context.ConvertToAction(iterator5->Get(2));

  EXPECT_TRUE(action.IsFinishedSuccessfully());

  iterator5.reset();
}

TEST_F(NonAtomicActionInterpreterTest, checkGotoSequence)
{
  // TODO(Semyon Protas): After using the scp-machine instead of the non-atomic action interpreter, check for execution
  // of goto together with else and true
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "finishedSubaction.scs");

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  initialize(context);

  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  shutdown(context);
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScAction action = getFirstAction(context);
  EXPECT_TRUE(action.IsFinished());

  ScIterator5Ptr iterator5 = context.CreateIterator5(
      action,
      ScType::ConstCommonArc,
      ScType::ConstNode,
      ScType::ConstPermPosArc,
      context.SearchElementBySystemIdentifier("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  action = context.ConvertToAction(iterator5->Get(2));

  EXPECT_TRUE(action.IsFinished());

  iterator5.reset();
}

TEST_F(NonAtomicActionInterpreterTest, checkArgumentsMatching)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "argumentsMatching.scs");
  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());

  initialize(context);
  ScAction testAction = context.ConvertToAction(testActionNode);
  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  shutdown(context);

  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScAction action = getFirstAction(context);
  EXPECT_TRUE(action.IsFinishedSuccessfully());

  ScTemplate scTemplate;
  scTemplate.Quintuple(
      action,
      ScType::VarPermPosArc,
      context.SearchElementBySystemIdentifier("arg1"),
      ScType::VarPermPosArc,
      ScKeynodes::rrel_1);
  scTemplate.Quintuple(
      action,
      ScType::VarPermPosArc,
      context.SearchElementBySystemIdentifier("arg3"),
      ScType::VarPermPosArc,
      ScKeynodes::rrel_2);
  ScTemplateSearchResult results;
  context.SearchByTemplate(scTemplate, results);
  EXPECT_TRUE(results.Size() == 1);

  ScIterator5Ptr iterator5 = context.CreateIterator5(
      action,
      ScType::ConstCommonArc,
      ScType::ConstNode,
      ScType::ConstPermPosArc,
      context.SearchElementBySystemIdentifier("nrel_goto"));
  EXPECT_TRUE(iterator5->Next());
  action = context.ConvertToAction(iterator5->Get(2));
  EXPECT_TRUE(action.IsFinishedSuccessfully());

  scTemplate.Quintuple(
      action,
      ScType::VarPermPosArc,
      context.SearchElementBySystemIdentifier("arg2"),
      ScType::VarPermPosArc,
      ScKeynodes::rrel_1);
  context.SearchByTemplate(scTemplate, results);
  EXPECT_TRUE(results.Size() == 1);

  iterator5.reset();
}

}  // namespace commonTest
