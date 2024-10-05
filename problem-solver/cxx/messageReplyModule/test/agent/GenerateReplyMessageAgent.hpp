#pragma once

#include <sc-memory/sc_agent.hpp>

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

}  // namespace messageReplyModuleTest
