#include <sc-agents-common/utils/AgentUtils.hpp>
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include "keynodes/MessageReplyKeynodes.hpp"

#include "GenerateReplyMessageAgent.hpp"

using namespace messageReplyModuleTest;

SC_AGENT_IMPLEMENTATION(GenerateReplyMessageAgent)
{
  ScAddr actionAddr = m_memoryCtx.GetEdgeTarget(edgeAddr);
  if(!m_memoryCtx.HelperCheckEdge(
        commonModule::Keynodes::action_interpret_non_atomic_action, actionAddr, ScType::EdgeAccessConstPosPerm))
  {
    return SC_RESULT_OK;
  }

  if(!actionIsValid(actionAddr))
  {
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
  }

  ScAddr argsSet = utils::IteratorUtils::getFirstByOutRelation(
        & m_memoryCtx,
        actionAddr,
        scAgentsCommon::CoreKeynodes::rrel_2);
  ScAddr messageAddr = utils::IteratorUtils::getFirstByOutRelation(
        & m_memoryCtx,
        argsSet,
        scAgentsCommon::CoreKeynodes::rrel_1);

  ScTemplate scTemplate;
  scTemplate.TripleWithRelation(
      messageAddr,
      ScType::EdgeDCommonVar,
      ScType::NodeVar,
      ScType::EdgeAccessVarPosPerm,
      messageReplyModule::MessageReplyKeynodes::nrel_reply);
  ScTemplateParams templateParams;
  ScTemplateGenResult templateGenResult;
  m_memoryCtx.HelperGenTemplate(scTemplate, templateGenResult, templateParams);

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, true);
  return SC_RESULT_OK;
}

bool GenerateReplyMessageAgent::actionIsValid(const ScAddr & actionAddr)
{
  ScTemplate scTemplate;
  scTemplate.TripleWithRelation(
        actionAddr,
        ScType::EdgeAccessVarPosPerm,
        messageReplyModule::MessageReplyKeynodes::message_processing_program,
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::rrel_1);
  scTemplate.TripleWithRelation(
        actionAddr,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_args_set",
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::rrel_2);
  scTemplate.TripleWithRelation(
        "_args_set",
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_message",
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::rrel_1);
  scTemplate.Triple(
        messageReplyModule::MessageReplyKeynodes::concept_message,
        ScType::EdgeAccessVarPosPerm,
        "_message");
  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(scTemplate, searchResult);
  return searchResult.Size() == 1;
}
