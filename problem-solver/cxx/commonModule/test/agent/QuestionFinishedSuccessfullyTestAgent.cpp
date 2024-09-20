#include "test/keynodes/TestKeynodes.hpp"

#include "QuestionFinishedSuccessfullyTestAgent.hpp"

using namespace commonTest;

ScResult ActionFinishedSuccessfullyTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  return action.FinishSuccessfully();
}

ScAddr ActionFinishedSuccessfullyTestAgent::GetActionClass() const
{
  return TestKeynodes::successfully_finished_test_action;
}

