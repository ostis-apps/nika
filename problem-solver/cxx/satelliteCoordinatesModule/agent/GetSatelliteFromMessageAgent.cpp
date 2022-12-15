#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "utils/ActionUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include "manager/CoordinatesManager.hpp"

#include "GetSatelliteFromMessageAgent.hpp"

using namespace scAgentsCommon;

namespace satelliteCoordinatesAgentsModule
{
SC_AGENT_IMPLEMENTATION(GetSatelliteFromMessageAgent)
{
  ScAddr actionAddr = otherAddr;

  // обязательная часть в теле агента, служит фильтром
  // агентов по принадлежности к классу агентов
  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }

  // логирование начала работы агента
  SC_LOG_INFO("GetSatelliteFromMessageAgent started");

  ScAddr const & message = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, CoreKeynodes::rrel_1);
  if (!message.IsValid())
  {
    // поскольку первый аргумент не корректен, то
    // завершаем работу агента с ошибкой
    SC_LOG_ERROR("The message isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);

    return SC_RESULT_ERROR;
  }

  if (!m_memoryCtx.HelperCheckEdge(Keynodes::concept_message_about_coordinates_of_satellite, message, ScType::EdgeAccessConstPosPerm))
  {
    //сообщение не содержит в себе информацию про спутник
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, ScAddrVector(), true);
    return SC_RESULT_OK;
  }

  // получение rrel_entity у первого входного аргумента
  ScAddr const & satellite = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, message, Keynodes::rrel_entity);

  if (!satellite.IsValid())
  {
    // поскольку rrel_entity у первого входного аргумента не корректен, то
    // завершаем работу агента с ошибкой
    SC_LOG_ERROR("The satellite isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);

    return SC_RESULT_ERROR;
  }
  SC_LOG_DEBUG("GetSatelliteFromMessageAgent: satellite idtf = " + m_memoryCtx.HelperGetSystemIdtf(satellite));

  ScAddr dynamicSatellite =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, CoreKeynodes::rrel_2);
  const auto & edge = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, dynamicSatellite, satellite);

  ScAddrVector answer{satellite, edge};

  // агент отработал успешно
  SC_LOG_INFO("GetSatelliteFromMessageAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answer, true);
  return SC_RESULT_OK;
}

bool GetSatelliteFromMessageAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      Keynodes::action_get_satellite_from_message, actionAddr, ScType::EdgeAccessConstPosPerm);
}
}  // namespace satelliteCoordinatesAgentsModule