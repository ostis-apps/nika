#pragma once

#include <sc-memory/sc_agent.hpp>

class ActionUtils
{
public:
  static bool isActionDeactivated(ScAgentContext * context, ScAddr const & actionAddr);
  static ScAction CreateAction(
      ScAgentContext * context,
      ScAddr const & actionClass,
      ScAddrVector const & actionArguments);
};
