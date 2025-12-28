#pragma once

#include <sc-memory/sc_agent.hpp>

#include "manager/WitMessageTopicClassificationManager.hpp"

namespace messageClassificationModule
{
class WitMessageTopicClassificationAgent : public ScActionInitiatedAgent
{
public:
  WitMessageTopicClassificationAgent();

  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  std::unique_ptr<WitMessageTopicClassificationManager> manager;

  void initFields();
};

}  // namespace messageClassificationModule
