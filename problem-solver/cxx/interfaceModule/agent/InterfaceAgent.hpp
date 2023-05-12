#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "InterfaceAgent.generated.hpp"

namespace interfaceModule
{
class InterfaceAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  int WAIT_TIME = 50000;

  bool checkActionClass(ScAddr const & actionAddr);

  std::string createColor();
};

}  // namespace interfaceModule
