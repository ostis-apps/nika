#include "TestModule.hpp"

using namespace testModule;

SC_IMPLEMENT_MODULE(TestModule)

sc_result TestModule::InitializeImpl()
{
  if (!testModule::TestKeynodes::InitGlobal())
  {
    return SC_RESULT_ERROR;
  }

  ScMemoryContext ctx(sc_access_lvl_make_min, "TestModule");
  if (ActionUtils::isActionDeactivated(&ctx, TestKeynodes::action_test_for_tpis))
  {
    SC_LOG_ERROR("action_test_for_tpis is deactivated");
  }
  else
  {
    SC_AGENT_REGISTER(TestAgent);
  }

  return SC_RESULT_OK;
}

sc_result TestModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(TestAgent);

  return SC_RESULT_OK;
}
