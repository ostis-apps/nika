#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "manager/MessageTopicClassificationManager.hpp"

#include "generated/MessageTopicClassificationAgent.generated.hpp"

namespace messageClassificationModule
{
class MessageTopicClassificationAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  std::unique_ptr<MessageTopicClassificationManager> manager;

  bool checkActionClass(ScAddr const & actionAddr);

  void initFields();
};

}  // namespace messageClassificationModule
