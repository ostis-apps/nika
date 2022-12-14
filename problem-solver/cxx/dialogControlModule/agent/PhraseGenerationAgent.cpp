#include <regex>

#include "handler/LinkHandler.hpp"
#include "manager/TemplateManager.hpp"
#include "keynodes/CoreKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "searcher/LanguageSearcher.hpp"
#include "utils/ActionUtils.hpp"
#include "utils/ScTemplateUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "PhraseGenerationAgent.hpp"

using namespace std;

using namespace inference;
using namespace utils;

namespace dialogControlModule
{
SC_AGENT_IMPLEMENTATION(PhraseGenerationAgent)
{
  ScAddr actionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);

  if (!checkActionClass(actionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("PhraseGenerationAgent started");

  ScAddr replyMessageNode =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!replyMessageNode.IsValid())
  {
    SC_LOG_ERROR("Action doesn't have a reply message node.");
    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  ScAddr phraseLink =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_2);
  if (!phraseLink.IsValid())
  {
    SC_LOG_ERROR("Action doesn't have a link with a text template.");
    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScAddr templateNode =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, phraseLink, CoreKeynodes::nrel_phrase_template);
  ScAddr parametersNode =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_3);

  ScAddr linkResult = generateLinkByTemplate(templateNode, parametersNode, phraseLink);
  if (!linkResult.IsValid())
  {
    SC_LOG_ERROR("Answer isn't found.");
    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    return SC_RESULT_ERROR;
  }
  LanguageSearcher searcher(&m_memoryCtx);
  ScAddr langNode = searcher.getLanguage(phraseLink);
  if (langNode.IsValid())
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, langNode, linkResult);
  }
  else
  {
    SC_LOG_DEBUG("Language link isn't found.");
  }
  if (!templateNode.IsValid())
  {
    updateSemanticAnswer(phraseLink);
  }
  generateSemanticEquivalent(replyMessageNode, templateNode);

  AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, linkResult, true);
  SC_LOG_DEBUG("PhraseGenerationAgent finished");
  return SC_RESULT_OK;
}

bool PhraseGenerationAgent::checkActionClass(const ScAddr & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
      MessageKeynodes::action_phrase_generation, actionNode, ScType::EdgeAccessConstPosPerm);
}

ScAddr PhraseGenerationAgent::generateLinkByTemplate(
    const ScAddr & templateNode,
    const ScAddr & parametersNode,
    const ScAddr & textTemplateLink)
{
  commonModule::LinkHandler handler(&m_memoryCtx);
  string textResult;
  ScAddr linkResult;

  string text = CommonUtils::getLinkContent(&m_memoryCtx, textTemplateLink);
  vector<string> variables = findTemplateVariables(text);
  if (!variables.empty())
  {
    textResult = findResultText(templateNode, parametersNode, variables, text);
  }
  else
  {
    SC_LOG_DEBUG("Text template doesn't have variables.");
    textResult = text;
  }
  if (!textResult.empty())
  {
    linkResult = handler.createLink(textResult);
    SC_LOG_DEBUG("Generated text: \"" + utils::CommonUtils::getLinkContent(&m_memoryCtx, linkResult) + "\"");
  }

  return linkResult;
}

string PhraseGenerationAgent::findResultText(
    const ScAddr & templateNode,
    const ScAddr & parametersNode,
    const vector<string> & variables,
    const string & text)
{
  string textResult;
  if (templateNode.IsValid())
  {
    vector<ScTemplateParams> parametersList = findParametersList(templateNode, parametersNode);
    for (auto & parameters : parametersList)
    {
      textResult = processScTemplate(templateNode, parameters, variables, text);
      if (!textResult.empty())
      {
        break;
      }
    }
  }
  else
  {
    SC_LOG_DEBUG("Phrase template isn't found.");
  }
  return textResult;
}

vector<ScTemplateParams> PhraseGenerationAgent::findParametersList(
    const ScAddr & templateNode,
    const ScAddr & parametersNode)
{
  TemplateManager manager(&m_memoryCtx);
  vector<ScTemplateParams> parametersList;
  if (parametersNode.IsValid())
  {
    ScAddrVector arguments = IteratorUtils::getAllWithType(&m_memoryCtx, parametersNode, ScType::Node);
    parametersList = manager.createTemplateParamsList(templateNode, arguments);
  }
  if (parametersList.empty())
  {
    parametersList.emplace_back();
  }
  return parametersList;
}

vector<string> PhraseGenerationAgent::findTemplateVariables(const string & text)
{
  vector<string> variables;
  regex regular(PhraseGenerationAgent::MAIN_VAR_REGULAR);
  smatch result;

  string::const_iterator searchStart(text.begin());
  while (regex_search(searchStart, text.cend(), result, regular))
  {
    string variable = result[0].str();
    variable = variable.substr(2, variable.length() - 3);
    variables.push_back(variable);
    searchStart = result.suffix().first;
  }

  return variables;
}

string PhraseGenerationAgent::processScTemplate(
    const ScAddr & templateNode,
    const ScTemplateParams & parameters,
    const vector<string> & variables,
    const string & text)
{
  string textResult;

  ScTemplate templateOption;
  if (m_memoryCtx.HelperBuildTemplate(templateOption, templateNode, parameters))
  {
    ScTemplateSearchResult phraseSemanticResult;
    if (m_memoryCtx.HelperSearchTemplate(templateOption, phraseSemanticResult))
    {
      textResult = this->generatePhraseAnswer(phraseSemanticResult, variables, text);
      this->updateSemanticAnswer(phraseSemanticResult);
    }
  }
  return textResult;
}

void PhraseGenerationAgent::generateSemanticEquivalent(const ScAddr & replyMessageNode, const ScAddr & templateNode)
{
  ScAddr semanticEquivalent = m_memoryCtx.CreateNode(ScType::NodeConstStruct);
  ScIterator3Ptr semanticEquivalentIterator =
      m_memoryCtx.Iterator3(MessageKeynodes::answer_structure, ScType::EdgeAccessConstPosPerm, ScType::Unknown);

  while (semanticEquivalentIterator->Next())
  {
    utils::GenerationUtils::addToSet(&m_memoryCtx, semanticEquivalent, semanticEquivalentIterator->Get(2));
  }

  ScTemplate semanticEquivalentStructure;
  semanticEquivalentStructure.TripleWithRelation(
      replyMessageNode,
      ScType::EdgeDCommonVar,
      semanticEquivalent,
      ScType::EdgeAccessVarPosPerm,
      MessageKeynodes::nrel_semantic_equivalent);
  ScTemplateGenResult result;
  m_memoryCtx.HelperGenTemplate(semanticEquivalentStructure, result);
}

string PhraseGenerationAgent::generatePhraseAnswer(
    const ScTemplateSearchResult & phraseSemanticResult,
    const std::vector<std::string> & variables,
    const std::string & text)
{
  string textResult = text;
  for (auto & variable : variables)
  {
    ScAddr link = phraseSemanticResult[0][variable];
    string linkValue = utils::CommonUtils::getLinkContent(&m_memoryCtx, link);
    string variableRegular =
        regex_replace(PhraseGenerationAgent::VAR_REGULAR, regex(PhraseGenerationAgent::VAR_CONST), variable);
    textResult = regex_replace(textResult, regex(variableRegular), linkValue);
  }
  return textResult;
}

void PhraseGenerationAgent::updateSemanticAnswer(const ScTemplateSearchResult & phraseSemanticResult)
{
  ScAddr phraseStruct = m_memoryCtx.CreateNode(ScType::NodeStruct);
  ScAddrVector phraseElements;
  for (size_t i = 0; i < phraseSemanticResult[0].Size(); i++)
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, phraseStruct, phraseSemanticResult[0][i]);
    phraseElements.push_back(phraseSemanticResult[0][i]);
  }

  ScAddrVector toRemoveNodes;
  toRemoveNodes.insert(toRemoveNodes.end(), NODES_TO_REMOVE.begin(), NODES_TO_REMOVE.end());
  for (auto node : NODES_TO_REMOVE_BY_CONCEPT)
  {
    addToRemoveNodes(phraseStruct, node, toRemoveNodes);
  }

  ScAddrVector toRemoveElements = toRemoveNodes;
  ScAddrVector buffElements;
  for (const auto & node : toRemoveNodes)
  {
    buffElements = this->getIncidentElements(node, phraseStruct);
    toRemoveElements.insert(toRemoveElements.end(), buffElements.begin(), buffElements.end());
  }

  for (auto & phraseElement : phraseElements)
  {
    // if (find(toRemoveElements.begin(), toRemoveElements.end(), phraseElement) == toRemoveElements.end())
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, phraseElement);
  }

  m_memoryCtx.EraseElement(phraseStruct);
}

void PhraseGenerationAgent::updateSemanticAnswer(const ScAddr & phraseAddr)
{
  ScAddrVector phraseElements;
  phraseElements.push_back(phraseAddr);

  ScIterator3Ptr const classesIt3 =
      m_memoryCtx.Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, phraseAddr);
  while (classesIt3->Next())
  {
    phraseElements.push_back(classesIt3->Get(0));
    phraseElements.push_back(classesIt3->Get(1));
  }

  ScIterator5Ptr const relationsIt5 = m_memoryCtx.Iterator5(
      phraseAddr, ScType::Unknown, ScType::Unknown, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  while (relationsIt5->Next())
  {
    phraseElements.push_back(relationsIt5->Get(1));
    phraseElements.push_back(relationsIt5->Get(2));
    phraseElements.push_back(relationsIt5->Get(3));
    phraseElements.push_back(relationsIt5->Get(4));
  }

  for (auto & phraseElement : phraseElements)
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, phraseElement);
  }
}

void PhraseGenerationAgent::addToRemoveNodes(
    const ScAddr & structNode,
    const ScAddr & conceptNode,
    ScAddrVector & vector)
{
  ScTemplate templateNode;
  templateNode.Triple(structNode, ScType::EdgeAccessVarPosPerm, ScType::Unknown >> "_node");
  templateNode.Triple(conceptNode, ScType::EdgeAccessVarPosPerm, "_node");

  ScTemplateSearchResult templateResult;
  m_memoryCtx.HelperSearchTemplate(templateNode, templateResult);

  for (size_t i = 0; i < templateResult.Size(); i++)
  {
    vector.push_back(templateResult[i]["_node"]);
  }
}

ScAddrVector PhraseGenerationAgent::getIncidentElements(const ScAddr & node, const ScAddr & structNode)
{
  ScAddrVector incidentElements;
  ScAddrVector buffElements;

  ScTemplate templateNode;
  templateNode.TripleWithRelation(
      node, ScType::EdgeDCommonVar >> "_remove_arc_1", ScType::Unknown, ScType::EdgeAccessVarPosPerm, structNode);
  buffElements = ScTemplateUtils::getAllWithKey(&m_memoryCtx, templateNode, "_remove_arc_1");
  incidentElements.insert(incidentElements.end(), buffElements.begin(), buffElements.end());

  templateNode.Clear();
  templateNode.TripleWithRelation(
      node, ScType::EdgeAccessVarPosPerm >> "_remove_arc_1", ScType::Unknown, ScType::EdgeAccessVarPosPerm, structNode);
  buffElements = ScTemplateUtils::getAllWithKey(&m_memoryCtx, templateNode, "_remove_arc_1");
  incidentElements.insert(incidentElements.end(), buffElements.begin(), buffElements.end());

  templateNode.Clear();
  templateNode.TripleWithRelation(
      ScType::Unknown, ScType::EdgeDCommonVar >> "_remove_arc_1", node, ScType::EdgeAccessVarPosPerm, structNode);
  buffElements = ScTemplateUtils::getAllWithKey(&m_memoryCtx, templateNode, "_remove_arc_1");
  incidentElements.insert(incidentElements.end(), buffElements.begin(), buffElements.end());

  templateNode.Clear();
  templateNode.TripleWithRelation(
      ScType::Unknown, ScType::EdgeAccessVarPosPerm >> "_remove_arc_1", node, ScType::EdgeAccessVarPosPerm, structNode);
  buffElements = ScTemplateUtils::getAllWithKey(&m_memoryCtx, templateNode, "_remove_arc_1");
  incidentElements.insert(incidentElements.end(), buffElements.begin(), buffElements.end());

  return incidentElements;
}
}  // namespace dialogControlModule
