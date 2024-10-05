#pragma once

#include <sc-memory/sc_agent.hpp>

namespace commonTest
{

class ActionFinishedUnsuccessfullyTestAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;
};

}  // namespace commonTest
