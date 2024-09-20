#pragma once

#include <sc-memory/sc_agent.hpp>
#include "keynodes/MessageReplyKeynodes.hpp"
namespace messageReplyModuleTest
{

class GenerateReplyMessageAgent : public ScActionInitiatedAgent
{
  public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

  private:
  bool actionIsValid(ScAddr const & actionAddr);
};

} // namespace commonTest
