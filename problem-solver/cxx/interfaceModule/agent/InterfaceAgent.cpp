#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "sc-memory/sc_link.hpp"

#include "utils/ActionUtils.hpp"
#include "keynodes/Keynodes.hpp"

#include "keynodes/InterfaceKeynodes.hpp"

#include "InterfaceAgent.hpp"

using namespace interfaceModule;
using namespace scAgentsCommon;

SC_AGENT_IMPLEMENTATION(InterfaceAgent)
{
  ScAddr actionAddr = otherAddr;
  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("InterfaceAgent started");

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, scAgentsCommon::CoreKeynodes::rrel_1);

  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
     return SC_RESULT_ERROR;
  }

  ScAddr const & componentAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, InterfaceKeynodes::rrel_entity);

  if (!componentAddr.IsValid())
  {
     SC_LOG_ERROR("Component Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
     return SC_RESULT_ERROR;
  }

  SC_LOG_DEBUG(ms_context->HelperGetSystemIdtf(componentAddr));
  SC_LOG_DEBUG(ms_context->HelperGetSystemIdtf(messageAddr));


  //std::string const messageText = getMessageText(messageAddr);


  std::string const COMPONENT_ALIAS = "_component";
  std::string const LINK_ALIAS = "_link";

  ScTemplate scTemplate;
  scTemplate.Triple(
    componentAddr,
    ScType::EdgeAccessVarPosPerm,
    ScType::NodeVar >> COMPONENT_ALIAS
  );
  scTemplate.TripleWithRelation(
    COMPONENT_ALIAS,
    ScType::EdgeDCommonVar,
    ScType::LinkVar >> LINK_ALIAS,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_component_color
  );

  ScTemplateSearchResult scResult;
  ms_context->HelperSearchTemplate(scTemplate, scResult);

  if (scResult.Size() == 1)
  {
    ScAddr scLinkAddr = scResult[0][LINK_ALIAS];
    ms_context->SetLinkContent(scLinkAddr, createColor());
  }

  SC_LOG_DEBUG("InterfaceAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, true);
  return SC_RESULT_OK;
}

bool InterfaceAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      InterfaceKeynodes::action_change_interface, actionAddr, ScType::EdgeAccessConstPosPerm);
}

std::string InterfaceAgent::createColor()
{
  std::string array[16] {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
  
  std::string answer = "#";
  for (int i=0; i<6; i++)
  { 
    int n = rand() % 15 + 0;
    answer += array[n];
  };
  return answer;
}