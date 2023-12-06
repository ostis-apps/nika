#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "TestAgent.generated.hpp"

namespace testModule
{
class TestAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkActionClass(ScAddr const & actionAddr);
  ScAddr getLinkConstructionLogin(std::string const & text);
  ScAddr getLinkConstructionPassword(std::string const & text);
  std::string GetTextAnswer(ScAddr const & questionTpisNode);

};

}  // namespace interfaceModule
