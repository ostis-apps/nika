#pragma once

#include <sc-memory/sc_agent.hpp>
#include "sc-agents-common/utils/IteratorUtils.hpp"

namespace commonTest
{

class AssignDynamicArgumentTestAgent : public ScActionInitiatedAgent
{
  public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

  };

} // namespace commonTest
