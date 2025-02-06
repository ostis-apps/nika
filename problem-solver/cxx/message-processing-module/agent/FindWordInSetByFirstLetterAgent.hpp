#pragma once

#include <sc-memory/sc_agent.hpp>

#include <common/searcher/MessageSearcher.hpp>

using namespace commonModule;

namespace messageProcessingModule
{
class FindWordInSetByFirstLetterAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  ScAddrVector createAnswer(std::string const & linkContent) const;

  std::string getMessageText(ScAddr const & messageAddr) const;

  std::unique_ptr<MessageSearcher> messageSearcher;
};
}  // namespace messageProcessingModule
