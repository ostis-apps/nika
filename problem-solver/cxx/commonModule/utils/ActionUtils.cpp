#include "ActionUtils.hpp"

#include "keynodes/Keynodes.hpp"

using namespace commonModule;

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
