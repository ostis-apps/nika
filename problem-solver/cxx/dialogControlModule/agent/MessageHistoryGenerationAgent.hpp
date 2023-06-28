#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "generator/MessageHistoryGenerator.hpp"

#include "MessageHistoryGenerationAgent.generated.hpp"

namespace dialogControlModule
{
class MessageHistoryGenerationAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkActionClass(const ScAddr & actionNode);
};
}  // namespace dialogControlModule
