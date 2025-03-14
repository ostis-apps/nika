#pragma once

#include <sc-memory/sc_agent.hpp>

#include "handler/MessageHandler.hpp"

namespace dialogControlModule
{
class StandardMessageReplyAgent : public ScActionInitiatedAgent
{
public:
  StandardMessageReplyAgent();

  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  int const DIRECT_INFERENCE_AGENT_WAIT_TIME = 15000;

  std::unique_ptr<LanguageSearcher> langSearcher;
  std::unique_ptr<MessageHandler> messageHandler;
  std::unique_ptr<commonModule::MessageSearcher> messageSearcher;

  ScAddr generatePhraseAgentParametersNode(const ScAddr & messageNode);

  ScAddr generateReplyMessage(const ScAddr & messageNode);

  ScAddr wrapInSet(ScAddr const & addr);

  void initFields();
};
}  // namespace dialogControlModule
