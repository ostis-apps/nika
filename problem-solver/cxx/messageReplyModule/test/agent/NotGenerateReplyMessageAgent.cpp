#include "NotGenerateReplyMessageAgent.hpp"

#include "keynodes/Keynodes.hpp"

using namespace messageReplyModuleTest;

ScResult NotGenerateReplyMessageAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  return action.FinishSuccessfully();
}

ScAddr NotGenerateReplyMessageAgent::GetActionClass() const
{
  return commonModule::Keynodes::action_interpret_non_atomic_action;
}
