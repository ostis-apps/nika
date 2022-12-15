#include "MathModule.hpp"

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_module.hpp"

#include "keynodes/Keynodes.hpp"
#include "agent/MathConstantsAgent.hpp"
#include "utils/ActionUtils.hpp"

namespace mathModule
{

SC_IMPLEMENT_MODULE(MathModule)

sc_result MathModule::InitializeImpl()
{

  if (!Keynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(MathConstantsAgent);
  return SC_RESULT_OK;
}

sc_result MathModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(MathConstantsAgent);
  return SC_RESULT_OK;
}

}
