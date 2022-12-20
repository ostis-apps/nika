#include "SatelliteCoordinatesModule.hpp"

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_module.hpp"

#include "keynodes/Keynodes.hpp"
#include "agent/GetSatelliteFromMessageAgent.hpp"
#include "agent/AddSatelliteCoordinatesAgent.hpp"
#include "agent/RemoveCoordinatesAgent.hpp"
#include "utils/ActionUtils.hpp"

namespace satelliteCoordinatesAgentsModule
{

// регистрация модуля
SC_IMPLEMENT_MODULE(SatelliteCoordinatesModule)

sc_result SatelliteCoordinatesModule::InitializeImpl()
{

  if (!Keynodes::InitGlobal())
    return SC_RESULT_ERROR;

  // регистрация агентов модуля
  SC_AGENT_REGISTER(GetSatelliteFromMessageAgent);
  SC_AGENT_REGISTER(AddSatelliteCoordinatesAgent);
  SC_AGENT_REGISTER(RemoveCoordinatesAgent);
  return SC_RESULT_OK;
}

sc_result SatelliteCoordinatesModule::ShutdownImpl()
{
  // дерегистрация агентов модуля
  SC_AGENT_UNREGISTER(GetSatelliteFromMessageAgent);
  SC_AGENT_UNREGISTER(AddSatelliteCoordinatesAgent);
  SC_AGENT_UNREGISTER(RemoveCoordinatesAgent);
  return SC_RESULT_OK;
}

}
