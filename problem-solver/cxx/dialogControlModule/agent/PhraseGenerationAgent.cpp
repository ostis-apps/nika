#include <regex>

#include "handler/LinkHandler.hpp"
#include "manager/templateManager/TemplateManager.hpp"
#include "keynodes/DialogKeynodes.hpp"
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
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, phraseLink, DialogKeynodes::nrel_phrase_template);
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
  std::map<VariableType, std::vector<std::string>> variables = getTemplateVariables(text);
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

std::map<VariableType, std::vector<std::string>> PhraseGenerationAgent::getTemplateVariables(std::string const & text)
{
  std::map<VariableType, std::vector<std::string>> variables;

  std::vector<std::string> linksIdentifiers = getTemplateLinksVariables(text);
  variables.insert(std::make_pair(LINK, linksIdentifiers));

  std::vector<std::string> setElementsIdentifiers = getTemplateSetElementsVariables(text);
  variables.insert(std::make_pair(SET_ELEMENTS, setElementsIdentifiers));

  return variables;
}

string PhraseGenerationAgent::findResultText(
    const ScAddr & templateNode,
    const ScAddr & parametersNode,
    std::map<VariableType, std::vector<std::string>> const & variables,
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
  ScAddrVector arguments = IteratorUtils::getAllWithType(&m_memoryCtx, parametersNode, ScType::Node);
  manager.setArguments(arguments);
  vector<ScTemplateParams> parametersList;
  if (parametersNode.IsValid())
  {
    parametersList = manager.createTemplateParams(templateNode);
  }
  if (parametersList.empty())
  {
    parametersList.emplace_back();
  }
  return parametersList;
}

vector<string> PhraseGenerationAgent::getTemplateLinksVariables(std::string const & text)
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

vector<string> PhraseGenerationAgent::getTemplateSetElementsVariables(std::string const & text)
{
  vector<string> variables;
  regex regularSetElements(PhraseGenerationAgent::SET_ELEMENTS_VAR_REGULAR);
  smatch setElementsResult;

  string::const_iterator searchSetElementsStart(text.begin());
  while (regex_search(searchSetElementsStart, text.cend(), setElementsResult, regularSetElements))
  {
    string variable = setElementsResult[0].str();
    variable = variable.substr(5, variable.length() - 6);
    variables.push_back(variable);
    searchSetElementsStart = setElementsResult.suffix().first;
  }

  return variables;
}

string PhraseGenerationAgent::processScTemplate(
    ScAddr const & templateNode,
    ScTemplateParams const & parameters,
    std::map<VariableType, std::vector<std::string>> const & variables,
    std::string const & text)
{
  string textResult;

  ScTemplate templateOption;
  if (m_memoryCtx.HelperBuildTemplate(templateOption, templateNode, parameters))
  {
    m_memoryCtx.HelperSmartSearchTemplate(
          templateOption,
          [this, &variables, &textResult, &text](ScTemplateSearchResultItem const & item) -> ScTemplateSearchRequest {
            textResult = generatePhraseAnswer(item, variables, text);
            updateSemanticAnswer(item);
            return ScTemplateSearchRequest::STOP;
          });
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
    ScTemplateSearchResultItem const & phraseSemanticResult,
    std::map<VariableType, std::vector<std::string>> const & variables,
    std::string const & text)
{
  string textResult = text;

  std::vector<std::string> linksIdentifiers = variables.at(LINK);
  replaceLinksVariables(phraseSemanticResult, linksIdentifiers, textResult);

  std::vector<std::string> setElementsIdentifiers = variables.at(SET_ELEMENTS);
  replaceSetElementsVariables(phraseSemanticResult, setElementsIdentifiers, textResult);

  return textResult;
}

void PhraseGenerationAgent::replaceLinksVariables(
    ScTemplateSearchResultItem const & phraseSemanticResult,
    std::vector<std::string> const & variables,
    std::string & text)
{
  for (std::string const & variable : variables)
  {
    if (!phraseSemanticResult.Has(variable))
    {
      text = "";
      break;
    }
    ScAddr link = phraseSemanticResult[variable];
    string linkValue = utils::CommonUtils::getLinkContent(&m_memoryCtx, link);
    string variableRegular =
          regex_replace(PhraseGenerationAgent::VAR_REGULAR, regex(PhraseGenerationAgent::VAR_CONST), variable);
    text = regex_replace(text, regex(variableRegular), linkValue);
  }
}

void PhraseGenerationAgent::replaceSetElementsVariables(
    ScTemplateSearchResultItem const & phraseSemanticResult,
    std::vector<std::string> const & variables,
    std::string & text)
{
  std::stringstream setElementsTextStream;
  for (std::string const & variable : variables)
  {
    if (!phraseSemanticResult.Has(variable))
    {
      text = "";
      break;
    }
    ScAddr set = phraseSemanticResult[variable];
    ScIterator3Ptr const & setElementsIterator = ms_context->Iterator3(set, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    if (setElementsIterator->Next())
    {
      setElementsTextStream << CommonUtils::getMainIdtf(&m_memoryCtx, setElementsIterator->Get(2), {scAgentsCommon::CoreKeynodes::lang_ru});
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, setElementsIterator->Get(1));
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, setElementsIterator->Get(2));
    }
    while (setElementsIterator->Next())
    {
      setElementsTextStream << ", " << CommonUtils::getMainIdtf(&m_memoryCtx, setElementsIterator->Get(2), {scAgentsCommon::CoreKeynodes::lang_ru});
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, setElementsIterator->Get(1));
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, setElementsIterator->Get(2));
    }

    string variableRegular =
          regex_replace(PhraseGenerationAgent::SET_ELEMENTS_VAR_REGULAR, regex(PhraseGenerationAgent::VAR_CONST), variable);
    text = regex_replace(text, regex(variableRegular), setElementsTextStream.str());
  }
}

void PhraseGenerationAgent::updateSemanticAnswer(const ScTemplateSearchResultItem & phraseSemanticResult)
{
  ScAddr phraseStruct = m_memoryCtx.CreateNode(ScType::NodeStruct);
  ScAddrVector phraseElements;
  for (size_t i = 0; i < phraseSemanticResult.Size(); i++)
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, phraseStruct, phraseSemanticResult[i]);
    phraseElements.push_back(phraseSemanticResult[i]);
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
