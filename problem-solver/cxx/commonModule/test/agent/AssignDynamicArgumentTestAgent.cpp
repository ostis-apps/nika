#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "test/keynodes/TestKeynodes.hpp"

#include "AssignDynamicArgumentTestAgent.hpp"

using namespace commonTest;

SC_AGENT_IMPLEMENTATION(AssignDynamicArgumentTestAgent)
{
  if (!edgeAddr.IsValid())
  {
    return SC_RESULT_ERROR;
  }

  ScAddr actionAddr = ms_context->GetEdgeTarget(edgeAddr);

  ScIterator3Ptr iterator3Ptr = ms_context->Iterator3(
        TestKeynodes::assign_dynamic_argument_test_action,
        ScType::EdgeAccessConstPosPerm,
        actionAddr);
  if (!iterator3Ptr->Next())
  {
    return SC_RESULT_OK;
  }

  ScAddr dynamicArgument = utils::IteratorUtils::getFirstByOutRelation(
        &m_memoryCtx,
        actionAddr,
        scAgentsCommon::CoreKeynodes::rrel_1);

  m_memoryCtx.CreateEdge(
        ScType::EdgeAccessConstPosTemp,
        dynamicArgument,
        TestKeynodes::test_node);

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, true);
  return SC_RESULT_OK;
}
