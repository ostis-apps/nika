#include "common/utils/ActionUtils.hpp"

#include "common/keynodes/Keynodes.hpp"

using namespace commonModule;

bool ActionUtils::isActionDeactivated(ScAgentContext * context, ScAddr const & action)
{
  return context->CheckConnector(commonModule::Keynodes::action_deactivated, action, ScType::ConstPermPosArc);
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

void ActionUtils::wrapActionResultToScStructure(
    ScAgentContext * context,
    ScAction & action,
    ScAddrVector const & answerElements)
{
  ScStructure result = context->GenerateStructure();
  for (auto const & element : answerElements)
    result << element;
  action.SetResult(result);
}
