#include "keynodes/DialogKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "utils/ActionUtils.hpp"
#include "utils/ScTemplateUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "MessageHistoryGenerationAgent.hpp"

using namespace std;

// using namespace inference;
using namespace utils;

namespace dialogControlModule
{
SC_AGENT_IMPLEMENTATION(MessageHistoryGenerationAgent)
{
  ScAddr actionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);
  MessageHistoryGenerator generator(&m_memoryCtx);
  if (!checkActionClass(actionNode))
  {

  SC_LOG_DEBUG("NOT MessageHistoryGenerationAgent started");

    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("MessageHistoryGenerationAgent started");

  ScAddr messageUser =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_1);

  if (!messageUser.IsValid())
  {
    SC_LOG_DEBUG("MessageHistoryGenerationAgent: the action doesn't have a message node");
    SC_LOG_DEBUG("MessageHistoryGenerationAgent finished");
    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  generator.addMessageToDialog(MessageKeynodes::concept_dialog_history, messageUser, MessageKeynodes::user);

  ScAddr messageNika =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_2);

  if (!messageNika.IsValid())
  {
    SC_LOG_DEBUG("MessageHistoryGenerationAgent: the action doesn't have a message node");
    SC_LOG_DEBUG("MessageHistoryGenerationAgent finished");
    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  generator.addMessageToDialog(MessageKeynodes::concept_dialog_history, messageNika, MessageKeynodes::nika);

  AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, true);
  SC_LOG_DEBUG("MessageHistoryGenerationAgent finished");
  return SC_RESULT_OK;
}

bool MessageHistoryGenerationAgent::checkActionClass(const ScAddr & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
      MessageKeynodes::action_message_history_generation, actionNode, ScType::EdgeAccessConstPosPerm);
}
}  // namespace dialogControlModule
