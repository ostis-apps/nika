#include "InterfaceModule.hpp"

using namespace interfaceModule;

SC_MODULE_REGISTER(InterfaceModule)->Agent<ChangeInterfaceColorAgent>();

// todo(codegen-removal): if needed override ScModule::Initialize and move all non-keynodes and non-agents code from
// previous initialization method
/*
{
  if (!interfaceModule::InterfaceKeynodes::InitGlobal())
  {
    return SC_RESULT_ERROR;
  }

  ScMemoryContext ctx(sc_access_lvl_make_min, "InterfaceModule");
  if (ActionUtils::isActionDeactivated(&ctx, InterfaceKeynodes::action_change_interface))
  {
    SC_LOG_ERROR("action_change_interface is deactivated");
  }
  else
  {
    //todo(codegen-removal): Use agentContext.SubscribeAgent<ChangeInterfaceColorAgent> or UnsubscribeAgent; to register
and unregister agent SC_AGENT_REGISTER(ChangeInterfaceColorAgent);
  }

  return SC_RESULT_OK;
}
*/

// todo(codegen-removal): if needed override ScModule::Shutdown and move all non-agents code from previous shutdown
// method
/*
{
  //todo(codegen-removal): Use agentContext.SubscribeAgent<ChangeInterfaceColorAgent> or UnsubscribeAgent; to register
and unregister agent SC_AGENT_UNREGISTER(ChangeInterfaceColorAgent);

  return SC_RESULT_OK;
}
*/
