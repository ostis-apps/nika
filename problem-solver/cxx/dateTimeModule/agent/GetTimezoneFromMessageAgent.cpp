#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "utils/ActionUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include "manager/DateTimeManager.hpp"

#include "GetTimezoneFromMessageAgent.hpp"

using namespace scAgentsCommon;

namespace dateTimeAgentsModule
{
SC_AGENT_IMPLEMENTATION(GetTimezoneFromMessageAgent)
{
  ScAddr actionAddr = otherAddr;

  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_INFO("GetTimezoneFromMessageAgent started");

  ScAddr const & message = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, CoreKeynodes::rrel_1);
  if (!message.IsValid())
  {
    SC_LOG_ERROR("The message isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);

    return SC_RESULT_ERROR;
  }

  if (!m_memoryCtx.HelperCheckEdge(Keynodes::concept_message_about_time_in_timezone, message, ScType::EdgeAccessConstPosPerm))
  {
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, ScAddrVector(), true);
    return SC_RESULT_OK;
  }

  ScAddr const & timezone = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, message, Keynodes::rrel_entity);

  if (!timezone.IsValid())
  {
    SC_LOG_ERROR("The timezone isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);

    return SC_RESULT_ERROR;
  }
  SC_LOG_DEBUG("GetTimezoneFromMessageAgent: timezone idtf = " + m_memoryCtx.HelperGetSystemIdtf(timezone));

  ScAddr dynamicTimezone =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, CoreKeynodes::rrel_2);
  const auto & edge = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, dynamicTimezone, timezone);

  ScAddrVector answer{timezone, edge};

  SC_LOG_INFO("GetTimezoneFromMessageAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answer, true);
  return SC_RESULT_OK;
}

bool GetTimezoneFromMessageAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      Keynodes::action_get_timezone_from_message, actionAddr, ScType::EdgeAccessConstPosPerm);
}
}  // namespace dateTimeAgentsModule