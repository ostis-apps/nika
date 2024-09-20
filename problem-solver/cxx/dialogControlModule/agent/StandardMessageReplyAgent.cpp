#include "StandardMessageReplyAgent.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "agent/DirectInferenceAgent.hpp"
#include "keynodes/InferenceKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace utils;

namespace dialogControlModule
{

ScResult StandardMessageReplyAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr messageNode = IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_1);

  if (!messageNode.IsValid())
  {
    SC_LOG_DEBUG("The action doesn't have a message node");
     return action.FinishUnsuccessfully();
  }

  ScAddr logicRuleNode = generateReplyMessage(messageNode);
  ScAddr replyMessageNode = IteratorUtils::getAnyByOutRelation(&m_context, messageNode, MessageKeynodes::nrel_reply);
  m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyMessageNode);

  if (!replyMessageNode.IsValid())
  {
    SC_LOG_ERROR("The reply message isn't generated");
    return action.FinishUnsuccessfully();
  }
  SC_LOG_DEBUG("The reply message is generated");

  auto * langSearcher = new LanguageSearcher(&m_context);
  ScAddr langNode = langSearcher->getMessageLanguage(messageNode);
  delete langSearcher;

  ScAddr parametersNode = generatePhraseAgentParametersNode(messageNode);

  auto * messageHandler = new MessageHandler(&m_context);
  if (!messageHandler->processReplyMessage(replyMessageNode, logicRuleNode, langNode, parametersNode))
  {
    SC_LOG_ERROR("The reply message is formed incorrectly");
    delete messageHandler;
    ScIterator5Ptr it5 = IteratorUtils::getIterator5(&m_context, replyMessageNode, MessageKeynodes::nrel_reply, false);
    if (it5->Next())
    {
      m_context.EraseElement(it5->Get(1));
    }

    return action.FinishUnsuccessfully();
  }

  ScAddr responseNode = IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_2);
  m_context.GenerateConnector(ScType::EdgeAccessConstPosTemp, responseNode, replyMessageNode);
  delete messageHandler;

  return action.FinishSuccessfully();
}

ScAddr StandardMessageReplyAgent::GetActionClass() const
{
  return MessageKeynodes::action_standard_message_reply;
}

ScAddr StandardMessageReplyAgent::generateReplyMessage(const ScAddr & messageNode)
{
  ScAddr logicRuleNode;
  ScAddrVector argsVector = {
      MessageKeynodes::template_reply_target,
      MessageKeynodes::concept_answer_on_standard_message_rule_class_by_priority,
      wrapInSet(messageNode)};

  ScAction actionDirectInference =
      ActionUtils::CreateAction(&m_context, inference::InferenceKeynodes::action_direct_inference, argsVector);
  bool const result = actionDirectInference.InitiateAndWait(DIRECT_INFERENCE_AGENT_WAIT_TIME);
  if (result)
  {
    ScAddr answer = IteratorUtils::getAnyByOutRelation(&m_context, actionDirectInference, ScKeynodes::nrel_result);
    ScAddr solutionNode = IteratorUtils::getAnyFromSet(&m_context, answer);
    ScAddr solutionTreeRoot = IteratorUtils::getAnyByOutRelation(&m_context, solutionNode, ScKeynodes::rrel_1);
    if (solutionTreeRoot.IsValid())
      logicRuleNode = IteratorUtils::getAnyByOutRelation(&m_context, solutionTreeRoot, ScKeynodes::rrel_1);
  }
  return logicRuleNode;
}

ScAddr StandardMessageReplyAgent::wrapInSet(ScAddr const & addr)
{
  ScAddr set = m_context.GenerateNode(ScType::NodeConstTuple);
  m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, set, addr);
  return set;
}

ScAddr StandardMessageReplyAgent::generatePhraseAgentParametersNode(const ScAddr & messageNode)
{
  ScAddrVector parameters;
  parameters.push_back(messageNode);

  auto * messageSearcher = new MessageSearcher(&m_context);

  ScAddr authorNode = messageSearcher->getMessageAuthor(messageNode);
  if (authorNode.IsValid())
    parameters.push_back(authorNode);

  ScAddr themeNode = messageSearcher->getMessageTheme(messageNode);
  if (themeNode.IsValid())
    parameters.push_back(themeNode);

  ScAddr parametersNode = m_context.GenerateNode(ScType::NodeConst);
  for (auto & node : parameters)
  {
    if (!m_context.CheckConnector(parametersNode, node, ScType::EdgeAccessConstPosPerm))
      m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, parametersNode, node);
  }

  delete messageSearcher;

  return parametersNode;
}
}  // namespace dialogControlModule
