#pragma once

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_agent.hpp>
#include <sc-memory/sc_memory.hpp>
using namespace std;

class ActionUtils
{
public:
  static bool waitAction(ScAgentContext * context, ScAddr const & actionAddr, int const & waitTime);
  static bool isActionDeactivated(ScAgentContext * context, ScAddr const & actionAddr);
  static ScAction CreateAction(
      ScAgentContext * context,
      ScAddr const & actionClass,
      ScAddrVector const & actionArguments);
};
