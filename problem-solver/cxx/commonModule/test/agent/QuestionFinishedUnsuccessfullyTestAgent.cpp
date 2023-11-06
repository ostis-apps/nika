#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "test/keynodes/TestKeynodes.hpp"

#include "QuestionFinishedUnsuccessfullyTestAgent.hpp"

using namespace commonTest;

SC_AGENT_IMPLEMENTATION(QuestionFinishedUnsuccessfullyTestAgent)
{
  if (!edgeAddr.IsValid())
  {
    return SC_RESULT_ERROR;
  }

  ScAddr actionAddr = ms_context->GetEdgeTarget(edgeAddr);

  ScIterator3Ptr iterator3Ptr = ms_context->Iterator3(
        TestKeynodes::unsuccessfully_finished_test_action,
        ScType::EdgeAccessConstPosPerm,
        actionAddr);
  if(!iterator3Ptr->Next())
  {
    return SC_RESULT_OK;
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
  return SC_RESULT_OK;
}
