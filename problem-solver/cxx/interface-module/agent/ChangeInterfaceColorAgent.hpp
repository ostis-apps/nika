#pragma once

#include <sc-memory/sc_agent.hpp>
namespace interfaceModule
{
class ChangeInterfaceColorAgent : public ScActionInitiatedAgent
{
public:
  ChangeInterfaceColorAgent();

  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  static std::string createColor();

  bool setComponentColor(ScAddr const & componentAddr, std::string const & componentColor);
};

}  // namespace interfaceModule
