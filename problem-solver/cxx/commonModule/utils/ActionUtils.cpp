#include "ActionUtils.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-memory/sc_wait.hpp"

#include "keynodes/Keynodes.hpp"

using namespace commonModule;

bool ActionUtils::waitAction(ScMemoryContext * context, ScAddr const & actionAddr, int const & waitTime)
{
  auto check = [](ScAddr const & listenAddr, ScAddr const & edgeAddr, ScAddr const & otherAddr) {
    return otherAddr == scAgentsCommon::CoreKeynodes::question_finished;
  };
  ScWaitCondition<ScEventAddInputEdge> waiter(*context, actionAddr, SC_WAIT_CHECK(check));
  return waiter.Wait(waitTime);
}

bool ActionUtils::isActionDeactivated(ScMemoryContext * context, ScAddr const & action)
{
  return context->HelperCheckEdge(commonModule::Keynodes::action_deactivated, action, ScType::EdgeAccessConstPosPerm);
}
