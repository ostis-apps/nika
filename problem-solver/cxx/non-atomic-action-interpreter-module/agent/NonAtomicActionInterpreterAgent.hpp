#pragma once

#include <sc-memory/sc_agent.hpp>

#include "interpreter/NonAtomicActionInterpreter.hpp"

namespace nonAtomicActionInterpreterModule
{
class NonAtomicActionInterpreterAgent : public ScActionInitiatedAgent
{
public:
  NonAtomicActionInterpreterAgent();

  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  std::unique_ptr<NonAtomicActionInterpreter> nonAtomicActionInterpreter;

  void generateNonAtomicActionTemplate(
      ScAddr const & nonAtomicActionTemplateAddr,
      ScTemplateParams const & templateParams);

  ScTemplateParams createTemplateParams(ScAddr const & nonAtomicActionTemplate, ScAddr const & argumentsSet);

  ScAddr getTemplateKeyElement(ScAddr const & templateAddr);

  ScAddr replaceNonAtomicAction(ScAddr const & templateAddr, ScTemplateParams & templateParams);

  void initFields();
};

}  // namespace nonAtomicActionInterpreterModule
