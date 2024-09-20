#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "test/keynodes/TestKeynodes.hpp"

#include "QuestionFinishedTestAgent.hpp"

using namespace commonTest;

ScResult ActionFinishedTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  return action.FinishSuccessfully();
}

ScAddr ActionFinishedTestAgent::GetActionClass() const
{
  return TestKeynodes::finished_test_action;
}

