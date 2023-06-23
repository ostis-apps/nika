#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "searcher/MessageSearcher.hpp"

#include "LetterAgent.generated.hpp"


namespace exampleModule
{
class LetterAgent : public ScAgent
{
  // Тип события условия инициирования sc-агента -- появление в sc-памяти выходящей дуги из `question_initiated` к действию
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))

  SC_GENERATED_BODY();

private:
  bool checkActionClass(ScAddr const & actionAddr);

	ScAddrVector createAnswer(ScAddr const & messageAddr, std::string const & linkContent, ScAddr const & relation) const;

  std::string getMessageText(ScAddr const & messageAddr) const;

  std::unique_ptr<class dialogControlModule::MessageSearcher> messageSearcher;
};
}  // namespace exampleModule
