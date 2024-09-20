#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include "keynodes/MessageReplyKeynodes.hpp"

#include "GenerateReplyMessageAgent.hpp"

using namespace messageReplyModuleTest;

//todo(codegen-removal): remove agent starting and finishing logs, sc-machine is printing them now
//todo(codegen-removal): if your agent is ScActionInitiatedAgent and uses event only to get action node via event.GetOtherElement() then you can remove event from method arguments and use ScAction & action instead of your action node
//todo(codegen-removal): if your agent is having method like CheckActionClass(ScAddr actionAddr) that checks connector between action class and actionAddr then you can remove it. Before agent is started sc-machine check that action belongs to class returned by GetActionClass()
//todo(codegen-removal): use action.SetResult() to pass result of your action instead of using answer or answerElements
//todo(codegen-removal): use SC_AGENT_LOG_SOMETHING() instead of SC_LOG_SOMETHING to automatically include agent name to logs messages
//todo(codegen-removal): use auto const & [names of action arguments] = action.GetArguments<amount of arguments>(); to get action arguments
ScResult GenerateReplyMessageAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr actionAddr = m_context.GetArcTargetElement(event.GetArc());
  if(!m_context.CheckConnector(
        commonModule::Keynodes::action_interpret_non_atomic_action, actionAddr, ScType::EdgeAccessConstPosPerm))
  {
    return action.FinishSuccessfully();
  }
  SC_LOG_DEBUG("GenerateMessageReplyAgent started");
  if(!actionIsValid(actionAddr))
  {
//todo(codegen-removal): replace AgentUtils:: usage
    utils::AgentUtils::finishAgentWork(&m_context, actionAddr, false);
  }

  ScAddr argsSet = utils::IteratorUtils::getFirstByOutRelation(
        & m_context,
        actionAddr,
        ScKeynodes::rrel_2);
  ScAddr messageAddr = utils::IteratorUtils::getFirstByOutRelation(
        & m_context,
        argsSet,
        ScKeynodes::rrel_1);

  ScTemplate scTemplate;
  scTemplate.TripleWithRelation(
      messageAddr,
      ScType::EdgeDCommonVar,
      ScType::NodeVar,
      ScType::EdgeAccessVarPosPerm,
      messageReplyModule::MessageReplyKeynodes::nrel_reply);
  ScTemplateParams templateParams;
  ScTemplateGenResult templateGenResult;
//todo(codegen-removal): method has signature changed
  m_context.GenerateByTemplate(scTemplate, templateGenResult, templateParams);
  SC_LOG_DEBUG("GenerateMessageReplyAgent finished");
//todo(codegen-removal): replace AgentUtils:: usage
  utils::AgentUtils::finishAgentWork(&m_context, actionAddr, true);
  return action.FinishSuccessfully();
}

ScAddr GenerateReplyMessageAgent::GetActionClass() const
{
//todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}


bool GenerateReplyMessageAgent::actionIsValid(const ScAddr & actionAddr)
{
  ScTemplate scTemplate;
  scTemplate.TripleWithRelation(
        actionAddr,
        ScType::EdgeAccessVarPosPerm,
        messageReplyModule::MessageReplyKeynodes::message_processing_program,
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_1);
  scTemplate.TripleWithRelation(
        actionAddr,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_args_set",
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_2);
  scTemplate.TripleWithRelation(
        "_args_set",
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_message",
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_1);
  scTemplate.Triple(
        messageReplyModule::MessageReplyKeynodes::concept_message,
        ScType::EdgeAccessVarPosPerm,
        "_message");
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(scTemplate, searchResult);
  return searchResult.Size() == 1;
}
