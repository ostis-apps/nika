#pragma once

#include <sc-memory/sc_agent.hpp>

#include "manager/MessageTopicClassificationManagerInterface.hpp"

namespace messageClassificationModule
{

class RasaMessageTopicClassificationAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  std::unique_ptr<MessageTopicClassificationManagerInterface> manager;

  void initFields();
};

}