#pragma once

#include <sc-memory/sc_agent.hpp>

namespace messageClassificationModule
{
class AlternativeMessageTopicClassificationAgent : public ScActionInitiatedAgent
{
public:
  AlternativeMessageTopicClassificationAgent();

  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;
};

}  // namespace messageClassificationModule
