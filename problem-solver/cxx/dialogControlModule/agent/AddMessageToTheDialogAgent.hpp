#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "generator/AddMessageToTheDialogGenerator.hpp"

#include "AddMessageToTheDialogAgent.generated.hpp"

namespace dialogControlModule
{
class AddMessageToTheDialogAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkActionClass(ScAddr const & actionNode);
};
}  // namespace dialogControlModule
