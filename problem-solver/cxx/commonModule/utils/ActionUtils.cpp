#include "ActionUtils.hpp"

#include "sc-memory/sc_event_wait.hpp"

#include "keynodes/Keynodes.hpp"

using namespace commonModule;

bool ActionUtils::waitAction(ScAgentContext * context, ScAddr const & actionAddr, int const & waitTime)
{
  auto waiter = context->CreateEventWaiter<ScEventAfterGenerateConnector<ScType::EdgeAccessConstPosPerm>>(actionAddr);
  return waiter->Wait(waitTime);
}

bool ActionUtils::isActionDeactivated(ScAgentContext * context, ScAddr const & action)
{
  return context->CheckConnector(commonModule::Keynodes::action_deactivated, action, ScType::EdgeAccessConstPosPerm);
}
