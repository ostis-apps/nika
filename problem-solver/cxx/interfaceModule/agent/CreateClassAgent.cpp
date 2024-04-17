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

#include "CreateClassAgent.hpp"

#include "generator/MessageConstructionsGenerator.hpp"

#include <vector>
#include <string>
#include <unordered_map>

using namespace interfaceModule;
using namespace scAgentsCommon;
using namespace dialogControlModule;
using namespace commonModule;


SC_AGENT_IMPLEMENTATION(CreateClassAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("CreateClassAgent started");

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("CreateClassAgent finished");
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

  if (formLinkContent == "User close")
  {
    ScAddr const & phrase = m_memoryCtx.HelperFindBySystemIdtf("concept_phrase_about_user_close");
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

    SC_LOG_DEBUG("CreateClassAgent finished : user close");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
    return SC_RESULT_OK;
  }

  std::vector<std::string> formItems = split(formLinkContent, "\n");
  /*bool checkExist = true, checkInvalid = false; 

  if (!formItems[4].empty())
  {
  std::vector<std::string> formItemsDiv = split(formItems[4], ", ");
  
  formItems[4]=formItemsDiv[0];

  for (size_t i=1; i<formItemsDiv.size(); i++)
    {
    formItems.push_back(formItemsDiv[i]);
    }


   ScAddr checkN;

  for (size_t i=0; i<formItemsDiv.size(); i++)
    {
    if (!m_memoryCtx.HelperFindBySystemIdtf(formItemsDiv[i], checkN))
      checkExist = false;
    if (formItemsDiv[i] == "concept_")
      checkInvalid = true;
    }
  }*/

  ScAddr checkNode;
  if (m_memoryCtx.HelperFindBySystemIdtf(formItems[0], checkNode))
  {
    createAnswerMessageAndStructure("concept_phrase_about_error_of_existing_class", answerStructure);
    
    SC_LOG_DEBUG("CreateClassAgent finished with error: nodes exists");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
    return SC_RESULT_OK;
    }

  if (formItems[0] == "concept_" || formItems[1] == "" || formItems[2] == "")
  {
    createAnswerMessageAndStructure("concept_phrase_about_error_invalid_params_for_creating_class", answerStructure);
    
    SC_LOG_DEBUG("CreateClassAgent finished with error: invalid inputs");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
    return SC_RESULT_OK;
  }


//Create Class Construction

  ScTemplate classConstruction;

  ScAddr const & classNode = m_memoryCtx.CreateNode(ScType::NodeConstClass);
  m_memoryCtx.HelperSetSystemIdtf(formItems[0], classNode);

  ScAddr const & classMainIdtf = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(classMainIdtf, formItems[1]);

  ScAddr const & NoteLink = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(NoteLink, formItems[2]);

  ScAddr superClassNode = m_memoryCtx.CreateNode(ScType::NodeConstClass);

  if(m_memoryCtx.HelperFindBySystemIdtf(formItems[3]).IsValid()){
    superClassNode = m_memoryCtx.HelperFindBySystemIdtf(formItems[3]);

    classConstruction.TripleWithRelation(
      superClassNode,
      ScType::EdgeDCommonVar,
      classNode,
      ScType::EdgeAccessVarPosPerm,
      InterfaceKeynodes::nrel_inclusion
    );
  }

  if(!formItems[4].empty()){

    ScAddrVector subdividingClasses = {};

    std::vector<std::string> chips = split(formItems[4], ", ");

    for (size_t i=0; i<chips.size(); i++){
      ScAddr const & subdividingClassNode = m_memoryCtx.HelperFindBySystemIdtf(chips[i]);
      if (subdividingClassNode.IsValid())
      {
        subdividingClasses.push_back(subdividingClassNode);
      }
    }

    if(!subdividingClasses.empty())
    {
      classConstruction.TripleWithRelation(
        ScType::NodeVarTuple >> "tuple",
        ScType::EdgeDCommonVar,
        classNode,
        ScType::EdgeAccessVarPosPerm,
        InterfaceKeynodes::nrel_subdividing
      );

      for (size_t i = 0; i < subdividingClasses.size(); ++i){
        classConstruction.Triple(
          "tuple",
          ScType::EdgeAccessVarPosPerm,
          subdividingClasses[i]
        );
      }
    }
  }


  classConstruction.Triple(
    InterfaceKeynodes::concept_wit_entity,
    ScType::EdgeAccessVarPosPerm,
    classNode
  );  

  classConstruction.TripleWithRelation(
    classNode,
    ScType::EdgeDCommonVar,
    NoteLink,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_note
  );  

  classConstruction.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    NoteLink
  );

  classConstruction.TripleWithRelation(
    classNode,
    ScType::EdgeDCommonVar,
    classMainIdtf,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_main_idtf
  );  

  ScTemplateGenResult genClassConstruction;
  m_memoryCtx.HelperGenTemplate(classConstruction, genClassConstruction);

  createAnswerMessageAndStructure("concept_phrase_about_successful_creating_class", answerStructure);


  for (size_t i = 0; i < genClassConstruction.Size(); ++i)
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, answerStructure, genClassConstruction[i]);
  }

  SC_LOG_DEBUG("CreateClassAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
  return SC_RESULT_OK;
}

bool CreateClassAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      InterfaceKeynodes::action_create_class, actionAddr, ScType::EdgeAccessConstPosPerm);
}

std::vector<std::string> CreateClassAgent::split(const string & s, const string & delimiter)
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

void CreateClassAgent::createAnswer(std::string message)
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

void CreateClassAgent::createAnswerMessageAndStructure(std::string conceptName, ScAddr const & answerStructure)
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