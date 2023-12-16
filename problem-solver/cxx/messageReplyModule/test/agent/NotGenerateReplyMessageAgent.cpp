#include <sc-agents-common/utils/AgentUtils.hpp>

#include "keynodes/Keynodes.hpp"

#include "NotGenerateReplyMessageAgent.hpp"

using namespace messageReplyModuleTest;

SC_AGENT_IMPLEMENTATION(NotGenerateReplyMessageAgent)
{
  ScAddr actionAddr = ms_context->GetEdgeTarget(edgeAddr);
  if(!m_memoryCtx.HelperCheckEdge(
        commonModule::Keynodes::action_interpret_non_atomic_action, actionAddr, ScType::EdgeAccessConstPosPerm))
  {
    return SC_RESULT_OK;
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, true);
  return SC_RESULT_OK;
}
