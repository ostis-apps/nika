#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "CreateClassAgent.generated.hpp"

namespace interfaceModule
{
class CreateClassAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkActionClass(ScAddr const & actionAddr);
  std::vector<std::string> split(const std::string & s, const std::string & delimiter);
  void createAnswer(std::string message);
  void createAnswerMessageAndStructure(std::string conceptName, ScAddr const & answerStructure);
};
}  // namespace interfaceModule
