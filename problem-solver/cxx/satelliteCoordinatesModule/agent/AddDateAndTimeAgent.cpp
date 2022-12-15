#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "utils/ActionUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include "manager/DateTimeManager.hpp"

#include "AddSatelliteCoordinatesAgent.hpp"

using namespace scAgentsCommon;

namespace satelliteCoordinatesAgentsModule
{
SC_AGENT_IMPLEMENTATION(AddSatelliteCoordinatesAgent)
{
  ScAddr actionAddr = otherAddr;

  // обязательная часть в теле агента, служит фильтром
  // агентов по принадлежности к классу агентов
  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }

  // логирование начала работы агента
  SC_LOG_INFO("AddSatelliteCoordinatesAgent started");

  ScAddr const & satelliteSet = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, CoreKeynodes::rrel_1);
  if (!satelliteSet.IsValid())
  {
    // поскольку первый аргумент не корректен, то
    // завершаем работу агента с ошибкой
    SC_LOG_ERROR("The satelliteSet isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);

    return SC_RESULT_ERROR;
  }

  const auto & satelliteIterator =
      m_memoryCtx.Iterator3(satelliteSet, ScType::EdgeAccessConstPosTemp, ScType::NodeConst);

  if (satelliteIterator->Next())
  {
    ScAddr const & satellite = satelliteIterator->Get(2);

    if (!satellite.IsValid())
    {
      // поскольку satellite  не корректен, то
      // завершаем работу агента с ошибкой
      SC_LOG_ERROR("The satellite isn’t valid");
      utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);

      return SC_RESULT_ERROR;
    }
    SC_LOG_DEBUG("AddSatelliteCoordinatesAgent: satellite idtf = " + m_memoryCtx.HelperGetSystemIdtf(satellite));

    ScAddrVector answer;
    try
    {
      answer = this->setCoordinates(satellite);
    }
    catch (utils::ScException & ex)
    {
      // в случае исключительной ситуации завершаем
      // работу агента с ошибкой
      SC_LOG_ERROR(ex.Message());
      utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
      return SC_RESULT_ERROR;
    }

    // агент отработал успешно
    SC_LOG_INFO("AddSatelliteCoordinatesAgent finished");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answer, true);
    return SC_RESULT_OK;
  }
  else
  {
    SC_LOG_ERROR("AddSatelliteCoordinatesAgent: cannot find satellite");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
  }
}

ScAddrVector AddSatelliteCoordinatesAgent::setCoordinates(ScAddr const & satellite)
{
  CoordinatesManager manager(&m_memoryCtx);
  return manager.manageAddition({satellite});
}

bool AddSatelliteCoordinatesAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      Keynodes::action_add_satellite_coordinates, actionAddr, ScType::EdgeAccessConstPosPerm);
}
}  // namespace satelliteCoordinatesAgentsModule