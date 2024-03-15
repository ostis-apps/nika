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

  ScAddr const & formLinkAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);

  std::string formLinkContent;
  m_memoryCtx.GetLinkContent(formLinkAddr, formLinkContent);

  m_memoryCtx.EraseElement(formLinkAddr);

  std::vector<std::string> formItems = split(formLinkContent, "\n");

  SC_LOG_ERROR(formLinkContent);

  //Create Message Class Construction

  ScTemplate messageClassConstruction;

  ScAddr const & classNode = m_memoryCtx.CreateNode(ScType::NodeConstClass);
  m_memoryCtx.HelperSetSystemIdtf(formItems[0], classNode);

  ScAddr const & classMainIdtf = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(classMainIdtf, formItems[1]);

  ScAddr const & witIntent = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(witIntent, formItems[2]);

  messageClassConstruction.Triple(
    InterfaceKeynodes::concept_intent_possible_class,
    ScType::EdgeAccessVarPosPerm,
    classNode
  );

  messageClassConstruction.TripleWithRelation(
    classNode,
    ScType::EdgeDCommonVar,
    classMainIdtf,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_main_idtf
  );

  messageClassConstruction.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    classMainIdtf
  );

  messageClassConstruction.TripleWithRelation(
    classNode,
    ScType::EdgeDCommonVar,
    witIntent,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_wit_ai_idtf
  );

  messageClassConstruction.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    witIntent
  );

  //Create Phrase Template

  ScAddr const & phraseTemplateNode = m_memoryCtx.CreateNode(ScType::NodeConstClass);
  m_memoryCtx.HelperSetSystemIdtf(formItems[3], phraseTemplateNode);

  ScAddr const & phraseTemplateMainIdtf = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(phraseTemplateMainIdtf, formItems[4]);

  messageClassConstruction.TripleWithRelation(
    InterfaceKeynodes::concept_phrase,
    ScType::EdgeDCommonVar,
    phraseTemplateNode,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_inclusion
  );

  messageClassConstruction.TripleWithRelation(
    phraseTemplateNode,
    ScType::EdgeDCommonVar,
    phraseTemplateMainIdtf,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_main_idtf
  );

  messageClassConstruction.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    phraseTemplateMainIdtf
  );

  std::vector<std::string> phrases = split(formItems[5], ", ");

  for (std::string phrase : phrases)
  {
    ScAddr const & phraseLink = m_memoryCtx.CreateLink(ScType::LinkConst);
    m_memoryCtx.SetLinkContent(phraseLink, phrase);

    messageClassConstruction.Triple(
      phraseTemplateNode,
      ScType::EdgeAccessVarPosPerm,
      phraseLink
    );

    messageClassConstruction.Triple(
      InterfaceKeynodes::lang_ru,
      ScType::EdgeAccessVarPosPerm,
      phraseLink
    );
  }

  //Create Logic Rule
  //First scg-contour
  messageClassConstruction.Triple(
    InterfaceKeynodes::concept_message,
    ScType::EdgeAccessVarPosPerm >> "edge_from_concept_message",
    InterfaceKeynodes::_message
  );

  messageClassConstruction.Triple(
    classNode,
    ScType::EdgeAccessVarPosPerm >> "edge_from_class_node",
    InterfaceKeynodes::_message
  );

  ScAddr const & structUp = m_memoryCtx.CreateNode(ScType::NodeConstStruct);

  messageClassConstruction.Triple(
    structUp,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::concept_message
  );

  messageClassConstruction.Triple(
    structUp,
    ScType::EdgeAccessVarPosPerm,
    "edge_from_concept_message"
  );

  messageClassConstruction.Triple(
    structUp,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::_message
  );

  messageClassConstruction.Triple(
    structUp,
    ScType::EdgeAccessVarPosPerm,
    classNode
  );

  messageClassConstruction.Triple(
    structUp,
    ScType::EdgeAccessVarPosPerm,
    "edge_from_class_node"
  );

  //Second contour
  messageClassConstruction.Triple(
    classNode,
    ScType::EdgeAccessVarPosPerm >> "edge_to_reply",
    InterfaceKeynodes::_reply_message
  );

  messageClassConstruction.Triple(
    InterfaceKeynodes::concept_atomic_message,
    ScType::EdgeAccessVarPosPerm >> "edge_from_atomic",
    InterfaceKeynodes::_reply_message
  );

  messageClassConstruction.TripleWithRelation(
    InterfaceKeynodes::_message,
    ScType::EdgeDCommonVar >> "edge_from_message_to_reply",
    InterfaceKeynodes::_reply_message,
    ScType::EdgeAccessVarPosPerm >> "edge_from_nrel_reply",
    InterfaceKeynodes::nrel_reply
  );

  ScAddr const & structDown = m_memoryCtx.CreateNode(ScType::NodeConstStruct);

  messageClassConstruction.Triple(
    structDown,
    ScType::EdgeAccessVarPosPerm,
    classNode
  );

  messageClassConstruction.Triple(
    structDown,
    ScType::EdgeAccessVarPosPerm,
    "edge_to_reply"
  );

  messageClassConstruction.Triple(
    structDown,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::_reply_message
  );

  messageClassConstruction.Triple(
    structDown,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::concept_atomic_message
  );

  messageClassConstruction.Triple(
    structDown,
    ScType::EdgeAccessVarPosPerm,
    "edge_from_atomic"
  );

  messageClassConstruction.Triple(
    structDown,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::_message
  );

  messageClassConstruction.Triple(
    structDown,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_reply
  );

  messageClassConstruction.Triple(
    structDown,
    ScType::EdgeAccessVarPosPerm,
    "edge_from_message_to_reply"
  );

  messageClassConstruction.Triple(
    structDown,
    ScType::EdgeAccessVarPosPerm,
    "edge_from_nrel_reply"
  );
  //Create Rule

  messageClassConstruction.Triple(
    InterfaceKeynodes::atomic_logical_formula,
    ScType::EdgeAccessVarPosPerm,
    structUp
  );
  
  messageClassConstruction.Triple(
    InterfaceKeynodes::atomic_logical_formula,
    ScType::EdgeAccessVarPosPerm,
    structDown
  );

  messageClassConstruction.TripleWithRelation(
    structUp,
    ScType::EdgeDCommonVar >> "key_sc_element",
    structDown,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_implication
  );

  ScAddr const & ruleNode = m_memoryCtx.CreateNode(ScType::NodeConst);
  
  std::string replace = "concept";
  size_t pos = formItems[0].find(replace);
    if (pos != std::string::npos) {
        formItems[0].replace(pos, replace.length(), "lr");
    }
  
  m_memoryCtx.HelperSetSystemIdtf(formItems[0], ruleNode);

  messageClassConstruction.TripleWithRelation(
    ruleNode,
    ScType::EdgeAccessVarPosPerm,
    "key_sc_element",
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::rrel_main_key_sc_element
  );

  messageClassConstruction.Triple(
    InterfaceKeynodes::concept_answer_on_standart_message_rule_priority_1,
    ScType::EdgeAccessVarPosPerm,
    ruleNode
  );

  messageClassConstruction.TripleWithRelation(
    ruleNode,
    ScType::EdgeDCommonVar,
    ScType::NodeVarTuple >> "tuple",
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_answer_pattern
  );

  messageClassConstruction.TripleWithRelation(
    "tuple",
    ScType::EdgeAccessVarPosPerm,
    phraseTemplateNode,
    ScType::EdgeAccessVarPosPerm,
    scAgentsCommon::CoreKeynodes::rrel_1
  );

  ScTemplateGenResult full_construction;
  m_memoryCtx.HelperGenTemplate(messageClassConstruction, full_construction);

  SC_LOG_DEBUG("CreateAnswerTemplateAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode);
  return SC_RESULT_OK;
}



std::vector<std::string> CreateAnswerTemplateAgent::split(const string & s, const string & delimiter)
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

bool CreateAnswerTemplateAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      InterfaceKeynodes::action_create_question_class_and_phrase_template, actionAddr, ScType::EdgeAccessConstPosPerm);
}