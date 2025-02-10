#include "GenerateReplyMessageAgent.hpp"

#include "keynodes/MessageReplyKeynodes.hpp"
#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace messageReplyModuleTest;

ScResult GenerateReplyMessageAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  if (!actionIsValid(action))
  {
    return action.FinishUnsuccessfully();
  }

  ScAddr argsSet = action.GetArgument(ScKeynodes::rrel_2);
  ScAddr messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_context, argsSet, ScKeynodes::rrel_1);

  ScTemplate scTemplate;
  scTemplate.Quintuple(
      messageAddr,
      ScType::VarCommonArc,
      ScType::VarNode,
      ScType::VarPermPosArc,
      messageReplyModule::MessageReplyKeynodes::nrel_reply);
  ScTemplateParams templateParams;
  ScTemplateGenResult templateGenResult;

  m_context.GenerateByTemplate(scTemplate, templateGenResult, templateParams);

  return action.FinishSuccessfully();
}

ScAddr GenerateReplyMessageAgent::GetActionClass() const
{
  return messageReplyModule::MessageReplyKeynodes::action_interpret_non_atomic_action;
}

bool GenerateReplyMessageAgent::actionIsValid(const ScAddr & actionAddr)
{
  ScTemplate scTemplate;
  scTemplate.Quintuple(
      actionAddr,
      ScType::VarPermPosArc,
      messageReplyModule::MessageReplyKeynodes::message_processing_program,
      ScType::VarPermPosArc,
      ScKeynodes::rrel_1);
  scTemplate.Quintuple(
      actionAddr, ScType::VarPermPosArc, ScType::VarNode >> "_args_set", ScType::VarPermPosArc, ScKeynodes::rrel_2);
  scTemplate.Quintuple(
      "_args_set", ScType::VarPermPosArc, ScType::VarNode >> "_message", ScType::VarPermPosArc, ScKeynodes::rrel_1);
  scTemplate.Triple(messageReplyModule::MessageReplyKeynodes::concept_message, ScType::VarPermPosArc, "_message");
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(scTemplate, searchResult);
  return searchResult.Size() == 1;
}
