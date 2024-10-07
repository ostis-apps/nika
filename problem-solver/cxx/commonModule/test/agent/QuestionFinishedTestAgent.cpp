#include "QuestionFinishedTestAgent.hpp"

#include "test/keynodes/TestKeynodes.hpp"

using namespace commonTest;

ScResult ActionFinishedTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  return action.FinishSuccessfully();
}

ScAddr ActionFinishedTestAgent::GetActionClass() const
{
  return TestKeynodes::finished_test_action;
}
