#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"

#include "keynodes/InterfaceKeynodes.hpp"

#include "TestAgent.hpp"

using namespace interfaceModule;
using namespace scAgentsCommon;

SC_AGENT_IMPLEMENTATION(TestAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("TestAgent started");

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("TestAgent finished");
     return SC_RESULT_ERROR;
  }

  ScAddr const & inputLink = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);
  std::string inputContent;
  m_memoryCtx.GetLinkContent(inputLink, inputContent);

  SC_LOG_ERROR(inputContent);

  SC_LOG_ERROR("WORK");

  SC_LOG_DEBUG("TestAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode);
  return SC_RESULT_OK;
}

bool TestAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      InterfaceKeynodes::action_create_class, actionAddr, ScType::EdgeAccessConstPosPerm);
}