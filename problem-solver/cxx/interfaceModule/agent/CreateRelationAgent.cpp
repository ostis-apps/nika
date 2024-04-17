#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "utils/ActionUtils.hpp"
#include "keynodes/InferenceKeynodes.hpp"

#include "keynodes/InterfaceKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"

#include "CreateRelationAgent.hpp"

#include "generator/MessageConstructionsGenerator.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>


using namespace interfaceModule;
using namespace scAgentsCommon;
using namespace dialogControlModule;
using namespace commonModule;

SC_AGENT_IMPLEMENTATION(CreateRelationAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("CreateRelationAgent started");

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("CreateRelationAgent finished");
     return SC_RESULT_ERROR;
  }

  //Clear answer structure
  ScAddr const & answerStructure = m_memoryCtx.HelperFindBySystemIdtf("answer_structure");
  
  ScTemplate findAnswerStructure;

  findAnswerStructure.Triple(
    answerStructure,
    ScType::EdgeAccessVarPosPerm >> "y",
    ScType::Unknown
  );

  ScTemplateSearchResult resultAnswerStructure;
  bool const is_success = m_memoryCtx.HelperSearchTemplate(findAnswerStructure, resultAnswerStructure);
  
  if (is_success)
  {
    for (size_t i = 0; i < resultAnswerStructure.Size(); ++i)
    {
      m_memoryCtx.EraseElement(resultAnswerStructure[i]["y"]);
    }
  }
  //------------------------------


  ScAddr const & formLinkAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);

  std::string formLinkContent;
  m_memoryCtx.GetLinkContent(formLinkAddr, formLinkContent);

  m_memoryCtx.EraseElement(formLinkAddr);

  std::vector<std::string> formItems = split(formLinkContent, "\n");
  
  ScTemplate relationConstruction;

  ScAddr const & relationNode = m_memoryCtx.CreateNode(ScType::NodeConstNoRole);
  m_memoryCtx.HelperSetSystemIdtf(formItems[0], relationNode);

  ScAddr const & mainIdtf = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(mainIdtf, formItems[1]);

  ScAddr const & definitionLink = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(definitionLink, formItems[2]);

  ScAddr const & firstDomainNode = m_memoryCtx.HelperFindBySystemIdtf(formItems[3]);

  ScAddr const & secondDomainNode = m_memoryCtx.HelperFindBySystemIdtf(formItems[4]);

  relationConstruction.Triple(
    InterfaceKeynodes::sc_node_norole_relation,
    ScType::EdgeAccessVarPosPerm,
    relationNode
  );

  relationConstruction.Triple(
    InterfaceKeynodes::relation,
    ScType::EdgeAccessVarPosPerm,
    relationNode
  );

  relationConstruction.TripleWithRelation(
    relationNode,
    ScType::EdgeDCommonVar,
    mainIdtf,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_main_idtf
  );

  relationConstruction.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    mainIdtf
  );

  relationConstruction.TripleWithRelation(
    relationNode,
    ScType::EdgeDCommonVar,
    definitionLink,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_definition
  );

  relationConstruction.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    definitionLink
  );

  relationConstruction.TripleWithRelation(
    relationNode,
    ScType::EdgeDCommonVar,
    firstDomainNode,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_first_domain
  );

  relationConstruction.TripleWithRelation(
    relationNode,
    ScType::EdgeDCommonVar,
    secondDomainNode,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_second_domain
  );

  relationConstruction.Triple(
    ScType::NodeVarTuple >> "_tuple",
    ScType::EdgeAccessVarPosPerm,
    firstDomainNode
  );

  relationConstruction.Triple(
    "_tuple",
    ScType::EdgeAccessVarPosPerm,
    secondDomainNode
  );

  relationConstruction.TripleWithRelation(
    relationNode,
    ScType::EdgeDCommonVar,
    ScType::NodeVar >> "_node",
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_definitional_domain
  );

  relationConstruction.TripleWithRelation(
    "_tuple",
    ScType::EdgeDCommonVar,
    "_node",
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_combination
  );

  std::vector<std::string> classification = split(formItems[5], ", ");

  for(size_t i = 0; i < classification.size(); ++i)
  {
    ScAddr const & classificationConceptNode = m_memoryCtx.HelperFindBySystemIdtf(classification[i]);
    relationConstruction.Triple(
      classificationConceptNode,
      ScType::EdgeAccessVarPosPerm,
      relationNode
    );
  }

  ScTemplateGenResult full_construction;
  m_memoryCtx.HelperGenTemplate(relationConstruction, full_construction);

  createAnswerMessageAndStructure("concept_phrase_about_successful_creating_relation", answerStructure);

  for (size_t i = 0; i < full_construction.Size(); ++i)
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, answerStructure, full_construction[i]);
  }

  SC_LOG_DEBUG("CreateRelationAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
  return SC_RESULT_OK;
}



std::vector<std::string> CreateRelationAgent::split(const string & s, const string & delimiter)
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

bool CreateRelationAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      InterfaceKeynodes::action_create_relation, actionAddr, ScType::EdgeAccessConstPosPerm);
}

void CreateRelationAgent::createAnswer(std::string message)
{
  dialogControlModule::MessageConstructionsGenerator messageConstructionGenerator = MessageConstructionsGenerator(&m_memoryCtx);
  ScAddr const & author = m_memoryCtx.HelperFindBySystemIdtf("myself");
  ScAddr const & dialogue = m_memoryCtx.HelperFindBySystemIdtf("concept_dialogue");
  ScTemplate findTarg;
  findTarg.Triple(
    dialogue,
    ScType::EdgeAccessVarPosPerm,
    ScType::NodeVar >> "target"
  );
  ScTemplateSearchResult result;
  bool const isFoundByTemplate = m_memoryCtx.HelperSearchTemplate(findTarg, result);
  ScAddr const & targAddr = result[0]["target"];
  
  ScAddr const &replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
  messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, InterfaceKeynodes::lang_ru, message);
  utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, replyAddr, author, InterfaceKeynodes::nrel_authors);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, targAddr, replyAddr);
}

void CreateRelationAgent::createAnswerMessageAndStructure(std::string conceptName, ScAddr const & answerStructure)
{
  ScAddr const & phrase = m_memoryCtx.HelperFindBySystemIdtf(conceptName);
  ScTemplate searchPhraseConstruction;
  searchPhraseConstruction.TripleWithRelation(
    InterfaceKeynodes::concept_phrase,
    ScType::EdgeDCommonVar,
    phrase,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_inclusion
  );
  searchPhraseConstruction.Triple(
    phrase,
    ScType::EdgeAccessVarPosPerm,
    ScType::LinkVar >> "_link"
  );
  searchPhraseConstruction.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    "_link"
  );

  ScTemplateSearchResult phraseConstruction;
  m_memoryCtx.HelperSearchTemplate(searchPhraseConstruction, phraseConstruction);
  std::string answer;
  m_memoryCtx.GetLinkContent(phraseConstruction[0]["_link"], answer);
  createAnswer(answer);

  for (size_t i = 0; i < phraseConstruction[0].Size(); ++i)
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, answerStructure, phraseConstruction[0][i]);
  }
}