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
  bool checkActionClass(ScAddr const & actionAddr);

  static std::string createColor();

  bool setComponentColor(ScAddr const & componentAddr, std::string const & componentColor);
};

}  // namespace interfaceModule
