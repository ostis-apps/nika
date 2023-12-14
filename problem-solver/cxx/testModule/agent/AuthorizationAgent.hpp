#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "AuthorizationAgent.generated.hpp"

namespace testModule
{
class AuthorizationAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  ScAddrVector getAnswer(ScAddr const & user);
  bool checkActionClass(ScAddr const & actionAddr);
};

}  // namespace interfaceModule
