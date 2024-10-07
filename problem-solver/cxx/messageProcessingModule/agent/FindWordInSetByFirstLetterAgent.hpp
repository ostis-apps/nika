#pragma once

#include <sc-memory/sc_agent.hpp>

#include "searcher/MessageSearcher.hpp"

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

  std::unique_ptr<class dialogControlModule::MessageSearcher> messageSearcher;
};
}  // namespace messageProcessingModule
