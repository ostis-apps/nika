#pragma once

#include "handler/MessageHandler.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-memory/kpm/sc_agent.hpp"

#include "StandardMessageReplyAgent.generated.hpp"

namespace dialogControlModule
{
class StandardMessageReplyAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  const int DIRECT_INFERENCE_AGENT_WAIT_TIME = 15000;

  bool checkActionClass(const ScAddr & actionNode);

  ScAddr generatePhraseAgentParametersNode(const ScAddr & messageNode);

  ScAddr generateReplyMessage(const ScAddr & messageNode);

  ScAddr wrapInSet(ScAddr const & addr);

  ScAddr getLogicRule(const ScAddr & messageNode);
};
}  // namespace dialogControlModule
