#include "NotGenerateReplyMessageAgent.hpp"

using namespace messageReplyModuleTest;

ScResult NotGenerateReplyMessageAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  return action.FinishSuccessfully();
}

ScAddr NotGenerateReplyMessageAgent::GetActionClass() const
{
  return messageReplyModule::MessageReplyKeynodes::action_interpret_non_atomic_action;
}
