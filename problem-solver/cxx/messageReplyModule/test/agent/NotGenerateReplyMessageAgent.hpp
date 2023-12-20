#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "keynodes/MessageReplyKeynodes.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "NotGenerateReplyMessageAgent.generated.hpp"

namespace messageReplyModuleTest
{

class NotGenerateReplyMessageAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()
};

} // namespace commonTest
