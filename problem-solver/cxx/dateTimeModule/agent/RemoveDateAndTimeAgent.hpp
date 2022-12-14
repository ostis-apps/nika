#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "RemoveDateAndTimeAgent.generated.hpp"

namespace dateTimeAgentsModule
{

class RemoveDateAndTimeAgent : public ScAgent
{
  SC_CLASS(Agent, Event(
                      scAgentsCommon::CoreKeynodes::question_initiated,
                      ScEvent::Type::AddOutputEdge)
  )

  SC_GENERATED_BODY();


private:
  ScAddrVector deleteDateTime(ScAddr const & timezone);

  bool checkActionClass(ScAddr const & actionAddr);


};

}  // namespace dateTimeAgentsModule
