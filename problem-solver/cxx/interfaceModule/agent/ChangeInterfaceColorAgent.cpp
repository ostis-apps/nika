#include "ChangeInterfaceColorAgent.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/InterfaceKeynodes.hpp"

using namespace interfaceModule;
// todo(codegen-removal): remove agent starting and finishing logs, sc-machine is printing them now
// todo(codegen-removal): if your agent is ScActionInitiatedAgent and uses event only to get action node via
// event.GetOtherElement() then you can remove event from method arguments and use ScAction & action instead of your
// action node todo(codegen-removal): if your agent is having method like CheckActionClass(ScAddr actionAddr) that
// checks connector between action class and actionAddr then you can remove it. Before agent is started sc-machine check
// that action belongs to class returned by GetActionClass() todo(codegen-removal): use action.SetResult() to pass
// result of your action instead of using answer or answerElements todo(codegen-removal): use SC_AGENT_LOG_SOMETHING()
// instead of SC_LOG_SOMETHING to automatically include agent name to logs messages todo(codegen-removal): use auto
// const & [names of action arguments] = action.GetArguments<amount of arguments>(); to get action arguments
ScResult ChangeInterfaceColorAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    SC_LOG_ERROR("Message Addr not found.");
    return action.FinishUnsuccessfully();
  }

  ScAddr const & componentAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_context, messageAddr, InterfaceKeynodes::rrel_entity);
  if (!componentAddr.IsValid())
  {
    SC_LOG_INFO("Component Addr not found.");
    return action.FinishUnsuccessfully();
  }

  ScAddr const & componentColorAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_context, messageAddr, InterfaceKeynodes::rrel_color);
  std::string componentColor;
  if (!componentColorAddr.IsValid())
  {
    componentColor = createColor();
    SC_LOG_DEBUG("ChangeInterfaceColorAgent: component color is changed to random " << componentColor);
  }
  else
  {
    m_context.GetLinkContent(componentColorAddr, componentColor);
    SC_LOG_DEBUG("ChangeInterfaceColorAgent: component color is changed to " << componentColor);
  }

  bool isSuccess = setComponentColor(componentAddr, componentColor);

  return isSuccess ? action.FinishSuccessfully() : action.FinishUnsuccessfully();
}

ScAddr ChangeInterfaceColorAgent::GetActionClass() const
{
  return InterfaceKeynodes::action_change_interface;
}

std::string ChangeInterfaceColorAgent::createColor()
{
  std::string colorItems[16]{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};

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
  ScAddr const & componentElementAddr = utils::IteratorUtils::getAnyFromSet(&m_context, componentAddr);
  if (componentElementAddr.IsValid())
  {
    componentElementColorLink = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, componentElementAddr, InterfaceKeynodes::nrel_component_color);
  }
  if (componentElementColorLink.IsValid())
  {
    m_context.SetLinkContent(componentElementColorLink, componentColor);
    result = true;
  }

  return result;
}
