#include "QuestionFinishedUnsuccessfullyTestAgent.hpp"

#include "keynodes/TestKeynodes.hpp"

using namespace commonTest;

ScResult ActionFinishedUnsuccessfullyTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  return action.FinishUnsuccessfully();
}

ScAddr ActionFinishedUnsuccessfullyTestAgent::GetActionClass() const
{
  return TestKeynodes::unsuccessfully_finished_test_action;
}
