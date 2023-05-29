#include "keynodes/MessageKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "utils/ActionUtils.hpp"
#include "keynodes/InferenceKeynodes.hpp"
#include "StandardMessageReplyAgent.hpp"

using namespace utils;

namespace dialogControlModule
{
SC_AGENT_IMPLEMENTATION(StandardMessageReplyAgent)
{
  ScAddr actionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);

  if (!checkActionClass(actionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("StandardMessageReplyAgent started");

  ScAddr messageNode =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_1);

  if (!messageNode.IsValid())
  {
    SC_LOG_DEBUG("StandardMessageReplyAgent: the action doesn't have a message node");
    SC_LOG_DEBUG("StandardMessageReplyAgent finished");
    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScAddr logicRuleNode = generateReplyMessage(messageNode);
  ScAddr replyMessageNode = IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageNode, MessageKeynodes::nrel_reply);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyMessageNode);

  if (!replyMessageNode.IsValid())
  {
    SC_LOG_ERROR("StandardMessageReplyAgent: the reply message isn't generated");
    SC_LOG_DEBUG("StandardMessageReplyAgent finished");
    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    return SC_RESULT_ERROR;
  }
  SC_LOG_DEBUG("StandardMessageReplyAgent: the reply message is generated");

  auto * langSearcher = new LanguageSearcher(&m_memoryCtx);
  ScAddr langNode = langSearcher->getMessageLanguage(messageNode);
  delete langSearcher;

  ScAddr parametersNode = generatePhraseAgentParametersNode(messageNode);

  auto * messageHandler = new MessageHandler(&m_memoryCtx);
  if (!messageHandler->processReplyMessage(replyMessageNode, logicRuleNode, langNode, parametersNode))
  {
    SC_LOG_ERROR("StandardMessageReplyAgent: the reply message is formed incorrectly");
    delete messageHandler;
    ScIterator5Ptr it5 =
        IteratorUtils::getIterator5(&m_memoryCtx, replyMessageNode, MessageKeynodes::nrel_reply, false);
    if (it5->Next())
    {
      m_memoryCtx.EraseElement(it5->Get(1));
    }

    SC_LOG_DEBUG("StandardMessageReplyAgent finished");
    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    return SC_RESULT_ERROR;
  }

  ScAddr responseNode =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_2);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, responseNode, replyMessageNode);
  delete messageHandler;

  SC_LOG_DEBUG("StandardMessageReplyAgent finished");
  AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, replyMessageNode, true);
  return SC_RESULT_OK;
}

bool StandardMessageReplyAgent::checkActionClass(const ScAddr & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
      MessageKeynodes::action_standard_message_reply, actionNode, ScType::EdgeAccessConstPosPerm);
}

ScAddr StandardMessageReplyAgent::generateReplyMessage(const ScAddr & messageNode)
{
  ScAddr logicRuleNode;
  ScAddrVector argsVector = {
      MessageKeynodes::template_reply_target,
      MessageKeynodes::concept_answer_on_standard_message_rule_class_by_priority,
      wrapInSet(messageNode)};
  ScAddr actionDirectInference =
      utils::AgentUtils::initAgent(&m_memoryCtx, inference::InferenceKeynodes::action_direct_inference, argsVector);

  bool const result = ActionUtils::waitAction(&m_memoryCtx, actionDirectInference, DIRECT_INFERENCE_AGENT_WAIT_TIME);
  if (result)
  {
    ScAddr answer = IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionDirectInference, scAgentsCommon::CoreKeynodes::nrel_answer);
    ScAddr solutionNode = IteratorUtils::getAnyFromSet(&m_memoryCtx, answer);
    ScAddr solutionTreeRoot = IteratorUtils::getAnyByOutRelation(&m_memoryCtx, solutionNode, scAgentsCommon::CoreKeynodes::rrel_1);
    if (solutionTreeRoot.IsValid())
      logicRuleNode = IteratorUtils::getAnyByOutRelation(&m_memoryCtx, solutionTreeRoot, scAgentsCommon::CoreKeynodes::rrel_1);
  }
  return logicRuleNode;
}

ScAddr StandardMessageReplyAgent::wrapInSet(ScAddr const & addr)
{
  ScAddr set = m_memoryCtx.CreateNode(ScType::NodeConstTuple);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, set, addr);
  return set;
}

ScAddr StandardMessageReplyAgent::generatePhraseAgentParametersNode(const ScAddr & messageNode)
{
  ScAddrVector parameters;
  parameters.push_back(messageNode);

  auto * messageSearcher = new MessageSearcher(&m_memoryCtx);

  ScAddr authorNode = messageSearcher->getMessageAuthor(messageNode);
  if (authorNode.IsValid())
    parameters.push_back(authorNode);

  ScAddr themeNode = messageSearcher->getMessageTheme(messageNode);
  if (themeNode.IsValid())
    parameters.push_back(themeNode);

  ScAddr parametersNode = m_memoryCtx.CreateNode(ScType::NodeConst);
  for (auto & node : parameters)
  {
    if (!m_memoryCtx.HelperCheckEdge(parametersNode, node, ScType::EdgeAccessConstPosPerm))
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, parametersNode, node);
  }

  delete messageSearcher;

  return parametersNode;
}
}  // namespace dialogControlModule
