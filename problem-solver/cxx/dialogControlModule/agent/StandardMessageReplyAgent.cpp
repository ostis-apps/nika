#include "StandardMessageReplyAgent.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"

#include "keynodes/InferenceKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "keynodes/DialogKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace utils;

namespace dialogControlModule
{

ScResult StandardMessageReplyAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr messageNode = action.GetArgument(ScKeynodes::rrel_1);
  if (!messageNode.IsValid())                                     //create reply default message structure 
                                                                  //if it wasn't found by directInferenceAgent
  {
    SC_AGENT_LOG_DEBUG("The action doesn't have a message node");
  }

  ScAddr logicRuleNode = generateReplyMessage(messageNode);
  if (!m_context.IsElement(logicRuleNode))
  {
    SC_AGENT_LOG_WARNING("The reply message isn't generated because reply construction wasn't found through direct inference agent. Trying to generate default reply message");
    std::stringstream setElementsTextStream;
    ScAddrVector messageClasses;
    setElementsTextStream << "Извините, я не нашла ответа на Ваш вопрос. Я определила, что данное сообщение является элементом классов:";
    ScIterator3Ptr it3 = m_context.CreateIterator3(
    ScType::NodeConst,
    ScType::EdgeAccessConstPosPerm,
    messageNode);
    while (it3->Next())
    {
      messageClasses.push_back(it3->Get(0));
    }
    for (auto & messagClass : messageClasses)
    {
      if(CommonUtils::getMainIdtf(&m_context, messagClass, {ScKeynodes::lang_ru}) != "сообщение" && CommonUtils::getMainIdtf(&m_context, messagClass, {ScKeynodes::lang_ru}) != "")
      setElementsTextStream  << CommonUtils::getMainIdtf(&m_context, messagClass, {ScKeynodes::lang_ru}) <<", ";
    }
    ScAddr const & defaultReplyMessage = m_context.GenerateNode(ScType::NodeConst);
    ScAddr const & defaultReplyLink = m_context.GenerateLink(ScType::LinkConst);
    std::string replyText = setElementsTextStream.str();
    replyText[replyText.length() - 2] = '.';
    m_context.SetLinkContent(defaultReplyLink, replyText);
    ScTemplate T;
    T.Triple(
      ScKeynodes::lang_ru, 
      ScType::EdgeAccessVarPosPerm, 
      defaultReplyLink);
    T.Triple(
      ScType::NodeVar >> "_link",
      ScType::EdgeAccessVarPosPerm,
      defaultReplyLink
    );
    T.Quintuple(
      "_link", 
      ScType::EdgeDCommonVar, 
      defaultReplyMessage, 
      ScType::EdgeAccessVarPosPerm,
      DialogKeynodes::nrel_sc_text_translation
    );
    T.Quintuple(
      messageNode, 
      ScType::EdgeDCommonVar,
      defaultReplyMessage, 
      ScType::EdgeAccessVarPosPerm, 
      MessageKeynodes::nrel_reply
    );
    ScTemplateResultItem result;
    m_context.GenerateByTemplate(T, result);
    action.SetResult(defaultReplyMessage);
    return action.FinishSuccessfully();

  }
 ScAddr replyMessageNode = IteratorUtils::getAnyByOutRelation(&m_context, messageNode, MessageKeynodes::nrel_reply);
   if (!replyMessageNode.IsValid())
  {
    SC_AGENT_LOG_ERROR("The reply message isn't generated");
    return action.FinishUnsuccessfully();
  }
  else
  {
    if (m_context.GetElementType(replyMessageNode) == ScType::NodeConst)
    SC_AGENT_LOG_DEBUG("Reply message node is const");
  }
  m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyMessageNode);

  SC_AGENT_LOG_DEBUG("The reply message is generated");

  initFields();
  ScAddr langNode = langSearcher->getMessageLanguage(messageNode);

  ScAddr parametersNode = generatePhraseAgentParametersNode(messageNode);

  if (!messageHandler->processReplyMessage(replyMessageNode, logicRuleNode, langNode, parametersNode))
  {
    SC_AGENT_LOG_ERROR("The reply message is formed incorrectly");
    ScIterator5Ptr it5 = IteratorUtils::getIterator5(&m_context, replyMessageNode, MessageKeynodes::nrel_reply, false);
    if (it5->Next())
    {
      m_context.EraseElement(it5->Get(1));
    }

    return action.FinishUnsuccessfully();
  }
  ScAddr responseNode = action.GetArgument(ScKeynodes::rrel_2);

  if (m_context.IsElement(responseNode))
    m_context.GenerateConnector(ScType::EdgeAccessConstPosTemp, responseNode, replyMessageNode);

  action.SetResult(replyMessageNode);
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
    if (actionDirectInference.IsFinishedSuccessfully())
                                                           //checking if DicrectInferenceAgent
                                                           //performed successfully
    {
      ScAddr answer = IteratorUtils::getAnyByOutRelation(&m_context, actionDirectInference, ScKeynodes::nrel_result);
      ScAddr solutionNode = IteratorUtils::getAnyFromSet(&m_context, answer);
      ScAddr solutionTreeRoot = IteratorUtils::getAnyByOutRelation(&m_context, solutionNode, ScKeynodes::rrel_1);
      if (solutionTreeRoot.IsValid())
      {
        ScAddr argNode = IteratorUtils::getAnyFromSet(&m_context, solutionTreeRoot);
        ScAddrVector Arguments;
        ScIterator3Ptr ArgIt = m_context.CreateIterator3(argNode, ScType::EdgeAccessConstPosPerm, ScType::NodeVar);
        while (ArgIt ->Next())
        {
          Arguments.push_back(ArgIt->Get(2));
        }
        logicRuleNode = IteratorUtils::getAnyByOutRelation(&m_context, solutionTreeRoot, ScKeynodes::rrel_1);
      }
    }
    else
    {
      return logicRuleNode;
    }

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

  return parametersNode;
}

void StandardMessageReplyAgent::initFields()
{
  this->langSearcher = std::make_unique<LanguageSearcher>(&m_context);
  this->messageSearcher = std::make_unique<MessageSearcher>(&m_context);
  this->messageHandler = std::make_unique<MessageHandler>(&m_context);
}

}  // namespace dialogControlModule
