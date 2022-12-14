#include "DateTimeModule.hpp"

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_module.hpp"

#include "keynodes/Keynodes.hpp"
#include "agent/GetTimezoneFromMessageAgent.hpp"
#include "agent/AddDateAndTimeAgent.hpp"
#include "agent/RemoveDateAndTimeAgent.hpp"
#include "utils/ActionUtils.hpp"

namespace dateTimeAgentsModule
{

SC_IMPLEMENT_MODULE(DateTimeModule)

sc_result DateTimeModule::InitializeImpl()
{

  if (!Keynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(GetTimezoneFromMessageAgent)
  SC_AGENT_REGISTER(AddDateAndTimeAgent)
  SC_AGENT_REGISTER(RemoveDateAndTimeAgent)
  return SC_RESULT_OK;
}

sc_result DateTimeModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(GetTimezoneFromMessageAgent)
  SC_AGENT_UNREGISTER(AddDateAndTimeAgent)
  SC_AGENT_UNREGISTER(RemoveDateAndTimeAgent)
  return SC_RESULT_OK;
}

}
