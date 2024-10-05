#include "PhraseGenerationAgent.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "handler/LinkHandler.hpp"
#include "keynodes/DialogKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "manager/templateManager/TemplateManager.hpp"
#include "searcher/LanguageSearcher.hpp"
#include "utils/ScTemplateUtils.hpp"
#include <regex>

using namespace std;

using namespace inference;
using namespace utils;

namespace dialogControlModule
{

ScResult PhraseGenerationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr const & replyMessageNode = action.GetArgument(ScKeynodes::rrel_1);
  if (!m_context.IsElement(replyMessageNode))
  {
    SC_AGENT_LOG_ERROR("Action doesn't have a reply message node.");
    return action.FinishWithError();
  }
  ScAddr const & phraseLink = action.GetArgument(ScKeynodes::rrel_2);
  if (!m_context.IsElement(phraseLink))
  {
    SC_AGENT_LOG_ERROR("Action doesn't have a link with a text template.");
    return action.FinishWithError();
  }
  ScAddr templateNode =
      IteratorUtils::getAnyByOutRelation(&m_context, phraseLink, DialogKeynodes::nrel_phrase_template);
  ScAddr const & parametersNode = action.GetArgument(ScKeynodes::rrel_3);
  if (!m_context.IsElement(parametersNode))
  {
    SC_AGENT_LOG_ERROR("Action doesn't have a parameters node.");
    return action.FinishWithError();
  }

  ScAddr const & linkResult = generateLinkByTemplate(templateNode, parametersNode, phraseLink);
  if (!m_context.IsElement(linkResult))
  {
    SC_AGENT_LOG_ERROR("Answer isn't found.");
    return action.FinishUnsuccessfully();
  }
  LanguageSearcher searcher(&m_context);
  ScAddr const & langNode = searcher.getLanguage(phraseLink);
  if (m_context.IsElement(langNode))
  {
    m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, langNode, linkResult);
  }
  else
  {
    SC_AGENT_LOG_DEBUG("Language link isn't found.");
  }
  if (!m_context.IsElement(templateNode))
  {
    updateSemanticAnswer(phraseLink);
  }
  generateSemanticEquivalent(replyMessageNode, templateNode);

  action.SetResult(linkResult);

  return action.FinishSuccessfully();
}

ScAddr PhraseGenerationAgent::GetActionClass() const
{
  return MessageKeynodes::action_phrase_generation;
}

ScAddr PhraseGenerationAgent::generateLinkByTemplate(
    const ScAddr & templateNode,
    const ScAddr & parametersNode,
    const ScAddr & textTemplateLink)
{
  commonModule::LinkHandler handler(&m_context);
  string textResult;
  ScAddr linkResult;

  string text;
  m_context.GetLinkContent(textTemplateLink, text);
  std::map<VariableType, std::vector<std::string>> variables = getTemplateVariables(text);
  if (!variables.empty())
  {
    textResult = findResultText(templateNode, parametersNode, variables, text);
  }
  else
  {
    SC_AGENT_LOG_DEBUG("Text template doesn't have variables.");
    textResult = text;
  }
  if (!textResult.empty())
  {
    linkResult = handler.createLink(textResult);
    std::string linkContent;
    m_context.GetLinkContent(linkResult, linkContent);
    SC_AGENT_LOG_DEBUG("Generated text: \"" << linkContent << "\"");
  }
  return linkResult;
}

std::map<VariableType, std::vector<std::string>> PhraseGenerationAgent::getTemplateVariables(std::string const & text)
{
  std::map<VariableType, std::vector<std::string>> variables;

  std::vector<std::string> linksIdentifiers = getTemplateLinksVariables(text);
  if (!linksIdentifiers.empty())
  {
    variables.insert(std::make_pair(LINK, linksIdentifiers));
  }

  std::vector<std::string> setElementsIdentifiers = getTemplateSetElementsVariables(text);
  if (!setElementsIdentifiers.empty())
  {
    variables.insert(std::make_pair(SET_ELEMENTS, setElementsIdentifiers));
  }

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
    SC_AGENT_LOG_DEBUG("Phrase template isn't found.");
  }
  return textResult;
}

vector<ScTemplateParams> PhraseGenerationAgent::findParametersList(
    const ScAddr & templateNode,
    const ScAddr & parametersNode)
{
  TemplateManager manager(&m_context);
  ScAddrVector arguments = IteratorUtils::getAllWithType(&m_context, parametersNode, ScType::Node);
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

  m_context.BuildTemplate(templateOption, templateNode, parameters);
  m_context.SearchByTemplateInterruptibly(
      templateOption,
      [this, &variables, &textResult, &text](ScTemplateSearchResultItem const & item) -> ScTemplateSearchRequest {
        textResult = generatePhraseAnswer(item, variables, text);
        updateSemanticAnswer(item);
        return ScTemplateSearchRequest::STOP;
      });

  return textResult;
}

void PhraseGenerationAgent::generateSemanticEquivalent(const ScAddr & replyMessageNode, const ScAddr & templateNode)
{
  ScAddr semanticEquivalent = m_context.GenerateNode(ScType::NodeConstStruct);
  ScIterator3Ptr semanticEquivalentIterator =
      m_context.CreateIterator3(MessageKeynodes::answer_structure, ScType::EdgeAccessConstPosPerm, ScType::Unknown);

  while (semanticEquivalentIterator->Next())
  {
    utils::GenerationUtils::addToSet(&m_context, semanticEquivalent, semanticEquivalentIterator->Get(2));
  }

  ScTemplate semanticEquivalentStructure;
  semanticEquivalentStructure.Quintuple(
      replyMessageNode,
      ScType::EdgeDCommonVar,
      semanticEquivalent,
      ScType::EdgeAccessVarPosPerm,
      MessageKeynodes::nrel_semantic_equivalent);
  ScTemplateGenResult result;

  m_context.GenerateByTemplate(semanticEquivalentStructure, result);
}

string PhraseGenerationAgent::generatePhraseAnswer(
    ScTemplateSearchResultItem const & phraseSemanticResult,
    std::map<VariableType, std::vector<std::string>> const & variables,
    std::string const & text)
{
  string textResult = text;

  auto const & linksIdentifiers = variables.find(LINK);
  if (linksIdentifiers != variables.cend())
  {
    replaceLinksVariables(phraseSemanticResult, linksIdentifiers->second, textResult);
  }

  auto const & setElementsIdentifiers = variables.find(SET_ELEMENTS);
  if (setElementsIdentifiers != variables.cend())
  {
    replaceSetElementsVariables(phraseSemanticResult, setElementsIdentifiers->second, textResult);
  }

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
    string linkValue;
    m_context.GetLinkContent(link, linkValue);
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
    ScIterator3Ptr const & setElementsIterator =
        m_context.CreateIterator3(set, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    if (setElementsIterator->Next())
    {
      setElementsTextStream << CommonUtils::getMainIdtf(&m_context, setElementsIterator->Get(2), {ScKeynodes::lang_ru});
      m_context.GenerateConnector(
          ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, setElementsIterator->Get(1));
      m_context.GenerateConnector(
          ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, setElementsIterator->Get(2));
    }
    while (setElementsIterator->Next())
    {
      setElementsTextStream << ", "
                            << CommonUtils::getMainIdtf(&m_context, setElementsIterator->Get(2), {ScKeynodes::lang_ru});
      m_context.GenerateConnector(
          ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, setElementsIterator->Get(1));
      m_context.GenerateConnector(
          ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, setElementsIterator->Get(2));
    }

    string variableRegular = regex_replace(
        PhraseGenerationAgent::SET_ELEMENTS_VAR_REGULAR, regex(PhraseGenerationAgent::VAR_CONST), variable);
    text = regex_replace(text, regex(variableRegular), setElementsTextStream.str());
  }
}

void PhraseGenerationAgent::updateSemanticAnswer(const ScTemplateSearchResultItem & phraseSemanticResult)
{
  ScAddr const phraseStruct = m_context.GenerateNode(ScType::NodeStruct);
  ScAddrVector phraseElements;
  for (size_t i = 0; i < phraseSemanticResult.Size(); i++)
  {
    m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, phraseStruct, phraseSemanticResult[i]);
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
    m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, phraseElement);
  }

  m_context.EraseElement(phraseStruct);
}

void PhraseGenerationAgent::updateSemanticAnswer(const ScAddr & phraseAddr)
{
  ScAddrVector phraseElements;
  phraseElements.push_back(phraseAddr);

  ScIterator3Ptr const classesIt3 =
      m_context.CreateIterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, phraseAddr);
  while (classesIt3->Next())
  {
    phraseElements.push_back(classesIt3->Get(0));
    phraseElements.push_back(classesIt3->Get(1));
  }

  ScIterator5Ptr const relationsIt5 = m_context.CreateIterator5(
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
    m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, MessageKeynodes::answer_structure, phraseElement);
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
  m_context.SearchByTemplate(templateNode, templateResult);

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
  templateNode.Quintuple(
      node, ScType::EdgeDCommonVar >> "_remove_arc_1", ScType::Unknown, ScType::EdgeAccessVarPosPerm, structNode);
  buffElements = ScTemplateUtils::getAllWithKey(&m_context, templateNode, "_remove_arc_1");
  incidentElements.insert(incidentElements.end(), buffElements.begin(), buffElements.end());

  templateNode.Clear();
  templateNode.Quintuple(
      node, ScType::EdgeAccessVarPosPerm >> "_remove_arc_1", ScType::Unknown, ScType::EdgeAccessVarPosPerm, structNode);
  buffElements = ScTemplateUtils::getAllWithKey(&m_context, templateNode, "_remove_arc_1");
  incidentElements.insert(incidentElements.end(), buffElements.begin(), buffElements.end());

  templateNode.Clear();
  templateNode.Quintuple(
      ScType::Unknown, ScType::EdgeDCommonVar >> "_remove_arc_1", node, ScType::EdgeAccessVarPosPerm, structNode);
  buffElements = ScTemplateUtils::getAllWithKey(&m_context, templateNode, "_remove_arc_1");
  incidentElements.insert(incidentElements.end(), buffElements.begin(), buffElements.end());

  templateNode.Clear();
  templateNode.Quintuple(
      ScType::Unknown, ScType::EdgeAccessVarPosPerm >> "_remove_arc_1", node, ScType::EdgeAccessVarPosPerm, structNode);
  buffElements = ScTemplateUtils::getAllWithKey(&m_context, templateNode, "_remove_arc_1");
  incidentElements.insert(incidentElements.end(), buffElements.begin(), buffElements.end());

  return incidentElements;
}
}  // namespace dialogControlModule
