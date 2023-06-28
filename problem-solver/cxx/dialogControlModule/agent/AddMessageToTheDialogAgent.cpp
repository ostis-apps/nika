#include "keynodes/MessageKeynodes.hpp"
#include "utils/ActionUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "AddMessageToTheDialogAgent.hpp"

using namespace utils;

namespace dialogControlModule
{
SC_AGENT_IMPLEMENTATION(AddMessageToTheDialogAgent)
{
  ScAddr actionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);
  if (!checkActionClass(actionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("AddMessageToTheDialogAgent started");

  ScAddr message = IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_1);

  if (!message.IsValid())
  {
    SC_LOG_DEBUG("AddMessageToTheDialogAgent: the action doesn't have a message node");
    SC_LOG_DEBUG("AddMessageToTheDialogAgent finished");
    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScIterator3Ptr messageDynamicArgumentIterator =
      m_memoryCtx.Iterator3(message, ScType::EdgeAccessConstPosTemp, ScType::NodeConst);
  if (messageDynamicArgumentIterator->Next())
  {
    message = messageDynamicArgumentIterator->Get(2);
  }

  ScAddr dialogAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_2);

  AddMessageToTheDialogGenerator generator(&m_memoryCtx);
  try
  {
    generator.addMessageToDialog(dialogAddr, message);
  }
  catch (std::runtime_error & exception)
  {
    SC_LOG_ERROR(exception.what());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    SC_LOG_DEBUG("AddMessageToTheDialogAgent finished");
    return SC_RESULT_ERROR;
  }

  AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, true);
  SC_LOG_DEBUG("AddMessageToTheDialogAgent finished");
  return SC_RESULT_OK;
}

bool AddMessageToTheDialogAgent::checkActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
      MessageKeynodes::action_add_message_to_the_dialog, actionNode, ScType::EdgeAccessConstPosPerm);
}
}  // namespace dialogControlModule
