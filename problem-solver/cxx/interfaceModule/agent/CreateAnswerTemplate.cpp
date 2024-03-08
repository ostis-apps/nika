#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"

#include "keynodes/InterfaceKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"

#include "CreateAnswerTemplate.hpp"

#include "generator/MessageConstructionsGenerator.hpp"

#include <vector>
#include <string>
#include <unordered_map>

using namespace interfaceModule;
using namespace scAgentsCommon;
using namespace dialogControlModule;
using namespace commonModule;

SC_AGENT_IMPLEMENTATION(CreateAnswerTemplateAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("CreateAnswerTemplateAgent started");

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("CreateAnswerTemplateAgent finished");
     return SC_RESULT_ERROR;
  }

  dialogControlModule::MessageConstructionsGenerator messageConstructionGenerator = MessageConstructionsGenerator(&m_memoryCtx);

  ScAddr statusNode;


  while (m_memoryCtx.HelperFindBySystemIdtf("Success", statusNode) != SC_TRUE)
  {}
  m_memoryCtx.EraseElement(statusNode);

  std::vector<std::string> inputsList;
  std::string input;
  m_memoryCtx.GetLinkContent(m_memoryCtx.FindLinksByContentSubstring("System Identifier")[0], input);
  m_memoryCtx.EraseElement(m_memoryCtx.FindLinksByContentSubstring("System Identifier")[0]);
  inputsList.push_back(input);
  m_memoryCtx.GetLinkContent(m_memoryCtx.FindLinksByContentSubstring("Russian Identifier")[0], input);
  m_memoryCtx.EraseElement(m_memoryCtx.FindLinksByContentSubstring("Russian Identifier")[0]);
  inputsList.push_back(input);
  m_memoryCtx.GetLinkContent(m_memoryCtx.FindLinksByContentSubstring("English Identifier")[0], input);
  m_memoryCtx.EraseElement(m_memoryCtx.FindLinksByContentSubstring("English Identifier")[0]);
  inputsList.push_back(input);
  m_memoryCtx.GetLinkContent(m_memoryCtx.FindLinksByContentSubstring("Answer Templates")[0], input);
  m_memoryCtx.EraseElement(m_memoryCtx.FindLinksByContentSubstring("Answer Templates")[0]);
  inputsList.push_back(input);

  CreateAnswerTemplate inputForm;
  inputForm.updateObject(inputsList);

  ScAddr const & mainNode = m_memoryCtx.CreateNode(ScType::NodeConstClass);
  bool const & isTrue = m_memoryCtx.HelperSetSystemIdtf(inputForm.systemIndetifier, mainNode);

  ScAddr const & russianIdtfLink = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(russianIdtfLink, inputForm.russianIndetifier);

  ScAddr const & englishIdtfLink = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(englishIdtfLink, inputForm.englishIndetifier);

  ScTemplate createTemplate;
  
  createTemplate.TripleWithRelation(
    InterfaceKeynodes::concept_phrase,
    ScType::EdgeDCommonVar,
    mainNode,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_inclusion
  );

  createTemplate.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    russianIdtfLink
  );

  createTemplate.Triple(
    InterfaceKeynodes::lang_en,
    ScType::EdgeAccessVarPosPerm,
    englishIdtfLink
  );

  createTemplate.TripleWithRelation(
    mainNode,
    ScType::EdgeDCommonVar,
    russianIdtfLink,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_main_idtf
  );

  createTemplate.TripleWithRelation(
    mainNode,
    ScType::EdgeDCommonVar,
    englishIdtfLink,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_main_idtf
  );

  for (string phrase : inputForm.phrases)
  {
    ScAddr const & phraseLink = m_memoryCtx.CreateLink(ScType::LinkConst);
    m_memoryCtx.SetLinkContent(phraseLink, phrase);

    createTemplate.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    phraseLink
    );
    createTemplate.Triple(
      mainNode,
      ScType::EdgeAccessVarPosPerm,
      phraseLink
    );
  }


  ScTemplateGenResult answerPhraseConstruction;

  m_memoryCtx.HelperGenTemplate(createTemplate, answerPhraseConstruction);

  ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
  messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, InterfaceKeynodes::lang_ru, "Шаблоны ответов успешно созданы");
  utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);

  SC_LOG_DEBUG("CreateAnswerTemplateAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode);
  return SC_RESULT_OK;
}



std::vector<std::string> CreateAnswerTemplate::split(const string & s, const string & delimiter)
{
  std::vector<std::string> tokens;
    std::size_t start = 0;
    std::size_t end = s.find(delimiter);
    
    while (end != std::string::npos)
    {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    
    tokens.push_back(s.substr(start));
    
    return tokens;
}



void CreateAnswerTemplate::updateObject(std::vector<std::string> inputs)
{    
    for (string input : inputs)
    {
      std::vector<std::string> splitedInput = split(input, ": ");
      if (splitedInput[0] == "System Identifier")
        this->systemIndetifier = splitedInput[1];
      else if (splitedInput[0] == "Russian Identifier")
        this->russianIndetifier = splitedInput[1];
      else if (splitedInput[0] == "English Identifier")
        this->englishIndetifier = splitedInput[1];
      else if (splitedInput[0] == "Answer Templates")
        this->phrases = split(splitedInput[1], ", ");
    }
}


bool CreateAnswerTemplateAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      InterfaceKeynodes::action_create_answer_template, actionAddr, ScType::EdgeAccessConstPosPerm);
}