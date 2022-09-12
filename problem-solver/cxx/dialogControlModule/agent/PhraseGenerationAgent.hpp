#pragma once

#include <map>

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "PhraseGenerationAgent.generated.hpp"

namespace dialogControlModule
{
class PhraseGenerationAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  const std::string VAR_CONST = "var";
  const std::string VAR_REGULAR = R"(\$\{var\})";
  const std::string MAIN_VAR_REGULAR = R"(\$\{_\w+\})";

  const std::vector<ScAddr> NODES_TO_REMOVE = {
      MessageKeynodes::concept_message,
      MessageKeynodes::rrel_entity,
      scAgentsCommon::CoreKeynodes::lang_ru,
      scAgentsCommon::CoreKeynodes::nrel_main_idtf};
  const std::vector<ScAddr> NODES_TO_REMOVE_BY_CONCEPT = {
      MessageKeynodes::concept_message,
      scAgentsCommon::CoreKeynodes::lang_ru};

  bool checkActionClass(const ScAddr & actionNode);

  ScAddr generateLinkByTemplate(
      const ScAddr & templateNode,
      const ScAddr & parametersNode,
      const ScAddr & textTemplateLink);

  std::string findResultText(
      const ScAddr & templateNode,
      const ScAddr & parametersNode,
      const std::vector<std::string> & variables,
      const std::string & text);

  std::vector<ScTemplateParams> findParametersList(const ScAddr & templateNode, const ScAddr & parametersNode);

  std::vector<std::string> findTemplateVariables(const std::string & text);

  std::string processScTemplate(
      const ScAddr & templateNode,
      const ScTemplateParams & parameters,
      const std::vector<std::string> & variables,
      const std::string & text);

  std::string generatePhraseAnswer(
      const ScTemplateSearchResult & phraseSemanticResult,
      const std::vector<std::string> & variables,
      const std::string & text);

  void generateSemanticEquivalent(const ScAddr & replyMessageNode, const ScAddr & structure);

  void updateSemanticAnswer(const ScTemplateSearchResult & phraseSemanticResult);

  void updateSemanticAnswer(const ScAddr & phraseAddr);

  void addToRemoveNodes(const ScAddr & structNode, const ScAddr & conceptNode, ScAddrVector & vector);

  ScAddrVector getIncidentElements(const ScAddr & node, const ScAddr & structNode);
};
}  // namespace dialogControlModule
