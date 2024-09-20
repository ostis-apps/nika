#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "test/keynodes/TestKeynodes.hpp"

#include "QuestionFinishedTestAgent.hpp"

using namespace commonTest;

//todo(codegen-removal): remove agent starting and finishing logs, sc-machine is printing them now
//todo(codegen-removal): if your agent is ScActionInitiatedAgent and uses event only to get action node via event.GetOtherElement() then you can remove event from method arguments and use ScAction & action instead of your action node
//todo(codegen-removal): if your agent is having method like CheckActionClass(ScAddr actionAddr) that checks connector between action class and actionAddr then you can remove it. Before agent is started sc-machine check that action belongs to class returned by GetActionClass()
//todo(codegen-removal): use action.SetResult() to pass result of your action instead of using answer or answerElements
//todo(codegen-removal): use SC_AGENT_LOG_SOMETHING() instead of SC_LOG_SOMETHING to automatically include agent name to logs messages
//todo(codegen-removal): use auto const & [names of action arguments] = action.GetArguments<amount of arguments>(); to get action arguments
ScResult QuestionFinishedTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  if (!event.GetArc().IsValid())
  {
    return action.FinishUnsuccessfully();
  }

  ScAddr actionAddr = ms_context->GetArcTargetElement(event.GetArc());

  ScIterator3Ptr iterator3Ptr = ms_context->CreateIterator3(
        TestKeynodes::finished_test_action,
        ScType::EdgeAccessConstPosPerm,
        actionAddr);
  if(!iterator3Ptr->Next())
  {
    return action.FinishSuccessfully();
  }

  ms_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, ScKeynodes::question_finished, actionAddr);
  return action.FinishSuccessfully();
}

ScAddr QuestionFinishedTestAgent::GetActionClass() const
{
//todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

