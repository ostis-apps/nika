#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "utils/ActionUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include "manager/DateTimeManager.hpp"

#include "AddDateAndTimeAgent.hpp"

using namespace scAgentsCommon;

namespace dateTimeAgentsModule
{
SC_AGENT_IMPLEMENTATION(AddDateAndTimeAgent)
{
  ScAddr actionAddr = otherAddr;

  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_INFO("AddDateAndTimeAgent started");

  ScAddr const & timezoneSet = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, CoreKeynodes::rrel_1);
  if (!timezoneSet.IsValid())
  {
    SC_LOG_ERROR("The timezoneSet isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);

    return SC_RESULT_ERROR;
  }

  const auto & timezoneIterator =
      m_memoryCtx.Iterator3(timezoneSet, ScType::EdgeAccessConstPosTemp, ScType::NodeConst);

  if (timezoneIterator->Next())
  {
    ScAddr const & timezone = timezoneIterator->Get(2);

    if (!timezone.IsValid())
    {
      SC_LOG_ERROR("The timezone isn’t valid");
      utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);

      return SC_RESULT_ERROR;
    }
    SC_LOG_DEBUG("AddDateAndTimeAgent: timezone idtf = " + m_memoryCtx.HelperGetSystemIdtf(timezone));

    ScAddrVector answer;
    try
    {
      answer = this->setDateTime(timezone);
    }
    catch (utils::ScException & ex)
    {
      SC_LOG_ERROR(ex.Message());
      utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
      return SC_RESULT_ERROR;
    }

    SC_LOG_INFO("AddDateAndTimeAgent finished");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answer, true);
    return SC_RESULT_OK;
  }
  else
  {
    SC_LOG_ERROR("AddDateAndTimeAgent: cannot find timezone");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
  }
}

ScAddrVector AddDateAndTimeAgent::setDateTime(ScAddr const & timezone)
{
  DateTimeManager manager(&m_memoryCtx);
  return manager.manageAddition({timezone});
}

bool AddDateAndTimeAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      Keynodes::action_add_timezone_date_time, actionAddr, ScType::EdgeAccessConstPosPerm);
}
}  // namespace dateTimeAgentsModule