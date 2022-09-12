#include "CommonModule.hpp"

using namespace commonModule;

SC_IMPLEMENT_MODULE(CommonModule);

sc_result CommonModule::InitializeImpl()
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
    SC_AGENT_REGISTER(NonAtomicActionInterpreterAgent);
  }

  return SC_RESULT_OK;
}

sc_result CommonModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(NonAtomicActionInterpreterAgent);

  return SC_RESULT_OK;
}
