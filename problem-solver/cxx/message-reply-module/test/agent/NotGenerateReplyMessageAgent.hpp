#pragma once

#include <sc-memory/sc_agent.hpp>

#include "keynodes/MessageReplyKeynodes.hpp"
namespace messageReplyModuleTest
{

class NotGenerateReplyMessageAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;
};

}  // namespace messageReplyModuleTest
