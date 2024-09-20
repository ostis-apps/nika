#include "ActionUtils.hpp"

#include "sc-memory/sc_event_wait.hpp"

#include "keynodes/Keynodes.hpp"

using namespace commonModule;

bool ActionUtils::waitAction(ScAgentContext * context, ScAddr const & actionAddr, int const & waitTime)
{
  ScAction action = context->ConvertToAction(actionAddr);
  return action.InitiateAndWait(waitTime);
//  auto waiter = context->CreateEventWaiter<ScEventAfterGenerateConnector<ScType::EdgeAccessConstPosPerm>>(actionAddr);
//  return waiter->Wait(waitTime);
}

bool ActionUtils::isActionDeactivated(ScAgentContext * context, ScAddr const & action)
{
  return context->CheckConnector(commonModule::Keynodes::action_deactivated, action, ScType::EdgeAccessConstPosPerm);
}

ScAction ActionUtils::CreateAction(
    ScAgentContext * context,
    ScAddr const & actionClass,
    ScAddrVector const & actionArguments)
{
  ScAction action = context->GenerateAction(actionClass);
  size_t index = 0;
  for (ScAddr const argument : actionArguments)
    action.SetArgument(++index, argument);
  return action;
}
