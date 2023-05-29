#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "keynodes/InterfaceKeynodes.hpp"

#include "InterfaceAgent.hpp"

using namespace interfaceModule;
using namespace scAgentsCommon;

SC_AGENT_IMPLEMENTATION(InterfaceAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("InterfaceAgent started");

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("InterfaceAgent finished");
     return SC_RESULT_ERROR;
  }

  ScAddr const & componentAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, InterfaceKeynodes::rrel_entity);
  if (!componentAddr.IsValid())
  {
     SC_LOG_INFO("Component Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("InterfaceAgent finished");
     return SC_RESULT_ERROR;
  }

  ScAddr const & componentColorAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, InterfaceKeynodes::rrel_color);
  std::string componentColor;
  if (!componentColorAddr.IsValid())
  {
    componentColor = createColor();
  }
  else
  {
    ms_context->GetLinkContent(componentColorAddr, componentColor);
  }

  bool isSuccess = setComponentColor(componentAddr, componentColor);

  SC_LOG_DEBUG("InterfaceAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, isSuccess);
  return SC_RESULT_OK;
}

bool InterfaceAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      InterfaceKeynodes::action_change_interface, actionAddr, ScType::EdgeAccessConstPosPerm);
}

std::string InterfaceAgent::createColor()
{
  std::string colorItems[16]
  {"0", "1", "2", "3",
   "4","5", "6", "7",
   "8", "9","A", "B",
   "C", "D", "E", "F"};
  
  std::string color = "#";
  for (size_t i = 0; i < 6; i++)
  { 
    size_t randomIndex = rand() % 15 + 0;
    color += colorItems[randomIndex];
  }

  return color;
}

bool InterfaceAgent::setComponentColor(ScAddr const & componentAddr, std::string const & componentColor)
{
  bool result = false;
  ScAddr componentElementColorLink;
  ScAddr const & componentElementAddr = utils::IteratorUtils::getAnyFromSet(&m_memoryCtx, componentAddr);
  if (componentElementAddr.IsValid())
  {
    componentElementColorLink = utils::IteratorUtils::getAnyByOutRelation(
          &m_memoryCtx, componentElementAddr, InterfaceKeynodes::nrel_component_color);
  }
  if (componentElementColorLink.IsValid())
  {
    ms_context->SetLinkContent(componentElementColorLink, componentColor);
    result = true;
  }

  return result;
}
