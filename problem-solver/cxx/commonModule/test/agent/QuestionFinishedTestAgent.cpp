#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "test/keynodes/TestKeynodes.hpp"

#include "QuestionFinishedTestAgent.hpp"

using namespace commonTest;

SC_AGENT_IMPLEMENTATION(QuestionFinishedTestAgent)
{
  if (!edgeAddr.IsValid())
  {
    return SC_RESULT_ERROR;
  }

  ScAddr actionAddr = ms_context->GetEdgeTarget(edgeAddr);

  ScIterator3Ptr iterator3Ptr = ms_context->Iterator3(
        TestKeynodes::finished_test_action,
        ScType::EdgeAccessConstPosPerm,
        actionAddr);
  if(!iterator3Ptr->Next())
  {
    return SC_RESULT_OK;
  }

  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_finished, actionAddr);
  return SC_RESULT_OK;
}
