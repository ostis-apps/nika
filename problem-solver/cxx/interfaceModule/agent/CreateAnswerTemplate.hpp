#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "CreateAnswerTemplate.generated.hpp"

namespace interfaceModule
{
class CreateAnswerTemplateAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkActionClass(ScAddr const & actionAddr);
};

class CreateAnswerTemplate
{
public:
  std::string systemIndetifier = "";
  std::string russianIndetifier = "";
  std::string englishIndetifier = "";
  std::vector<std::string> phrases = {};

  std::vector<std::string> split(const std::string & s, const std::string & delimiter);
  void updateObject(std::vector<std::string> inputs);
};

}  // namespace interfaceModule
