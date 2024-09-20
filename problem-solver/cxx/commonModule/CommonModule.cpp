#include "CommonModule.hpp"

using namespace commonModule;

SC_MODULE_REGISTER(CommonModule)->Agent<NonAtomicActionInterpreterAgent>();
;

// todo(codegen-removal): if needed override ScModule::Initialize and move all non-keynodes and non-agents code from
// previous initialization method
/*
{
  if (!commonModule::Keynodes::InitGlobal())
  {
    return SC_RESULT_ERROR;
  }

  ScMemoryContext ctx(sc_access_lvl_make_min, "CommonModule");
  if (ActionUtils::isActionDeactivated(&ctx, Keynodes::action_interpret_non_atomic_action))
  {
    SC_LOG_ERROR("action_interpret_non_atomic_action is deactivated");
  }
  else
  {
    //todo(codegen-removal): Use agentContext.SubscribeAgent<NonAtomicActionInterpreterAgent> or UnsubscribeAgent; to
register and unregister agent SC_AGENT_REGISTER(NonAtomicActionInterpreterAgent);
  }

  return SC_RESULT_OK;
}
*/

// todo(codegen-removal): if needed override ScModule::Shutdown and move all non-agents code from previous shutdown
// method
/*
{
  //todo(codegen-removal): Use agentContext.SubscribeAgent<NonAtomicActionInterpreterAgent> or UnsubscribeAgent; to
register and unregister agent SC_AGENT_UNREGISTER(NonAtomicActionInterpreterAgent);

  return SC_RESULT_OK;
}
*/
