#pragma once

#include <sc-memory/sc_agent.hpp>

namespace messageReplyModule
{
class MessageReplyAgent : public ScActionInitiatedAgent
{
public:
  MessageReplyAgent();

  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  int WAIT_TIME = 50000;

  ScAddr getMessageProcessingProgram();

  ScAddr generateMessage(ScAddr const & authorAddr, ScAddr const & linkAddr);

  ScAddr generateNonAtomicActionArgsSet(ScAddr const & messageAddr);

  ScAddr generateAnswer(ScAddr const & messageAddr);

  bool linkIsValid(ScAddr const & linkAddr);

  bool soundLinkIsValid(ScAddr const & linkAddr);

  bool textLinkIsValid(ScAddr const & linkAddr);

  bool hasLanguage(ScAddr const & linkAddr);
};

}  // namespace messageReplyModule
