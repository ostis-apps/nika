#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "searcher/MessageSearcher.hpp"

#include "LetterAgent.generated.hpp"


namespace exampleModule
{
class LetterAgent : public ScAgent
{
  // условие инициирования агента (появление в sc-памяти
  // выходящей дуги из `question_initiated` к экземпляру  
  // агента
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))

  // описывает логику работы агента
  SC_GENERATED_BODY();

private:
  // вспомогательный метод проверки принадлежности экземпляра 
  // агента к его классу
  bool checkActionClass(ScAddr const & actionAddr);

	ScAddrVector addTempLinkWithRelation(ScAddr addr, const std::string & linkContent, ScAddr relation) const;

  std::string getMessageText(ScAddr const & messageAddr) const;

  std::unique_ptr<class dialogControlModule::MessageSearcher> messageSearcher;
};
}  // namespace exampleModule