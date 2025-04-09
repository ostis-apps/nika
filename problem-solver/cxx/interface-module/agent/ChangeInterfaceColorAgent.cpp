#include "ChangeInterfaceColorAgent.hpp"

#include "keynodes/InterfaceKeynodes.hpp"
#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace interfaceModule;

ChangeInterfaceColorAgent::ChangeInterfaceColorAgent()
{
  m_logger = utils::ScLogger(utils::ScLogger::ScLogType::Console, "", utils::ScLogLevel::Debug);
}

ScResult ChangeInterfaceColorAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr const & messageAddr = action.GetArgument(ScKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    m_logger.Error("Message Addr not found.");
    return action.FinishUnsuccessfully();
  }

  ScAddr const & componentAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_context, messageAddr, InterfaceKeynodes::rrel_entity);
  if (!componentAddr.IsValid())
  {
    m_logger.Info("Component Addr not found.");
    return action.FinishUnsuccessfully();
  }

  ScAddr const & componentColorAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_context, messageAddr, InterfaceKeynodes::rrel_color);
  std::string componentColor;
  if (!componentColorAddr.IsValid())
  {
    componentColor = createColor();
    m_logger.Debug("Component color is changed to random ", componentColor);
  }
  else
  {
    m_context.GetLinkContent(componentColorAddr, componentColor);
    m_logger.Debug("Component color is changed to ", componentColor);
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
