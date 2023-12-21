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

  if (ActionUtils::isActionDeactivated(&ctx, TestKeynodes::action_authorization))
  {
    SC_LOG_ERROR("action_authorization is deactivated");
  }
  else
  {
    SC_AGENT_REGISTER(AuthorizationAgent);
  }

  if (ActionUtils::isActionDeactivated(&ctx, TestKeynodes::action_registration))
  {
    SC_LOG_ERROR("action_registration is deactivated");
  }
  else
  {
    SC_AGENT_REGISTER(RegistrationAgent);
  }

  if (ActionUtils::isActionDeactivated(&ctx, TestKeynodes::action_testing))
  {
    SC_LOG_ERROR("action_testing is deactivated");
  }
  else
  {
    SC_AGENT_REGISTER(TestingAgent);
  }

  return SC_RESULT_OK;
}

sc_result TestModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(TestAgent);
  SC_AGENT_UNREGISTER(AuthorizationAgent);
  SC_AGENT_UNREGISTER(RegistrationAgent);
  SC_AGENT_UNREGISTER(TestingAgent);

  return SC_RESULT_OK;
}
