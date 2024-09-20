#include "keynodes/Keynodes.hpp"

#include "NotGenerateReplyMessageAgent.hpp"

using namespace messageReplyModuleTest;

ScResult NotGenerateReplyMessageAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr actionAddr = m_context.GetArcTargetElement(event.GetArc());
  if(!m_context.CheckConnector(
        commonModule::Keynodes::action_interpret_non_atomic_action, actionAddr, ScType::EdgeAccessConstPosPerm))
  {
    return action.FinishUnsuccessfully();
  }

  return action.FinishSuccessfully();
}

ScAddr NotGenerateReplyMessageAgent::GetActionClass() const
{
  return messageReplyModule::MessageReplyKeynodes::action_reply_to_message;
}

