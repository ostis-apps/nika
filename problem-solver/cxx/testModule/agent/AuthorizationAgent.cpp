#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "keynodes/TestKeynodes.hpp"

#include "ChangeInterfaceColorAgent.hpp"

using namespace testModule;
using namespace scAgentsCommon;

SC_AGENT_IMPLEMENTATION(ChangeInterfaceColorAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("ChangeInterfaceColorAgent started");

  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_authorization_status, utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1));

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("ChangeInterfaceColorAgent finished");
     return SC_RESULT_ERROR;
  }

  ScAddr const & componentAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, InterfaceKeynodes::rrel_entity);
  if (!componentAddr.IsValid())
  {
     SC_LOG_INFO("Component Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("ChangeInterfaceColorAgent finished");
     return SC_RESULT_ERROR;
  }

  ScAddr const & componentColorAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, InterfaceKeynodes::rrel_color);
  std::string componentColor;
  if (!componentColorAddr.IsValid())
  {
    componentColor = createColor();
    SC_LOG_DEBUG("ChangeInterfaceColorAgent: component color is changed to random " << componentColor);
  }
  else
  {
    ms_context->GetLinkContent(componentColorAddr, componentColor);
    SC_LOG_DEBUG("ChangeInterfaceColorAgent: component color is changed to " << componentColor);
  }

  bool isSuccess = setComponentColor(componentAddr, componentColor);

  SC_LOG_DEBUG("InterfaceAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, isSuccess);
  return SC_RESULT_OK;
}

std::string GetTextAnswer(ScAddr const &questionTpisNode)
{
  ScTemplate textQuestion;
  textQuestion.TripleWithRelation(
    ScType::NodeVar >> "_node",
    ScType::EdgeAcessVarPosPerm,
    questionTpisNode,
    ScType::EdgeAcessVarPosPerm,
    Keynode::rrel_key_sc_element
  );
  textQuestion.TripleWithRelation(
   ScType::NodeVar >> "_node_1",
   ScType::EdgeDCommonVar,
   "_node",
   ScType::EdgeAcessVarPosPerm,
   Keynode::nrel_text_sc_translation
  );
  textQuestion.TripleWithRelation(
   "_node_1",
   ScType::EdgeAcessVarPosPerm,
    ScType::LinkVar >> "link_text",
   ScType::EdgeAcessVarPosPerm,
   Keynode::rrel_example
  );

  ScTemplateSearchResult result;
  ScAddr link;
  if (context->HelperSearchTemplate(phraseTemplate, result))
     link = result[0]["link_text"];


  return text = utils::CommomUtils::getLinkContent(&m_memoryCtx, link);
}

bool ChangeInterfaceColorAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      InterfaceKeynodes::action_change_interface, actionAddr, ScType::EdgeAccessConstPosPerm);
}

std::string ChangeInterfaceColorAgent::createColor()
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

bool ChangeInterfaceColorAgent::setComponentColor(ScAddr const & componentAddr, std::string const & componentColor)
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
