#pragma once

#include <sc-memory/sc_agent.hpp>

#include "manager/MessageTopicClassificationManager.hpp"

namespace messageClassificationModule
{
class MessageTopicClassificationAgent : public ScActionInitiatedAgent
{
public:
  MessageTopicClassificationAgent();

  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  std::unique_ptr<MessageTopicClassificationManager> manager;

  void initFields();
};

}  // namespace messageClassificationModule
