#pragma once

#include <sc-memory/sc_agent.hpp>

#include "handler/MessageHandler.hpp"

namespace dialogControlModule
{
class StandardMessageReplyAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  const int DIRECT_INFERENCE_AGENT_WAIT_TIME = 15000;

  ScAddr generatePhraseAgentParametersNode(const ScAddr & messageNode);

  ScAddr generateReplyMessage(const ScAddr & messageNode);

  ScAddr wrapInSet(ScAddr const & addr);
};
}  // namespace dialogControlModule
