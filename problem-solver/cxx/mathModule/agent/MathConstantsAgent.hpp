#pragma once

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-memory/kpm/sc_agent.hpp"

#include "MathConstantsAgent.generated.hpp"
#include "manager/MathConstantsManager.hpp"

namespace mathModule
{

class MathConstantsAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))

  SC_GENERATED_BODY();

private:
  std::unique_ptr<MathConstantsManager> manager;

  bool checkActionClass(ScAddr const & actionAddr);

  void initFields();
};

}  // namespace mathModule
