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

#include "CreateClassInstanceAgent.hpp"

#include "generator/MessageConstructionsGenerator.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>


using namespace interfaceModule;
using namespace scAgentsCommon;
using namespace dialogControlModule;
using namespace commonModule;

SC_AGENT_IMPLEMENTATION(CreateClassInstanceAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("CreateClassInstanceAgent started");

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("CreateClassInstanceAgent finished");
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
    SC_LOG_ERROR("size of answer structure");
    SC_LOG_ERROR(resultAnswerStructure.Size());
    for (size_t i = 0; i < resultAnswerStructure.Size(); ++i)
    {
      SC_LOG_ERROR("For step");
      SC_LOG_ERROR(i);
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

    SC_LOG_DEBUG("CreateClassInstanceAgent finished : user close");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
    return SC_RESULT_OK;
  }

  
  std::vector<std::string> formItems = split(formLinkContent, "\n");

  if (m_memoryCtx.HelperFindBySystemIdtf(formItems[0]).IsValid() || formItems[1] == "" || formItems[2] == "")
  {
    createAnswerMessageAndStructure("concept_phrase_about_create_instance_error", answerStructure);
    SC_LOG_DEBUG("CreateClassInstanceAgent finished with error");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
    return SC_RESULT_OK;
  }

  
  ScTemplate instanceConstruction;

  ScAddr const & instanceNode = m_memoryCtx.CreateNode(ScType::NodeConst);
  m_memoryCtx.HelperSetSystemIdtf(formItems[0], instanceNode);

  ScAddr const & mainIdtf = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(mainIdtf, formItems[1]);

  ScAddr const & noteLink = m_memoryCtx.CreateLink(ScType::LinkConst);
  m_memoryCtx.SetLinkContent(noteLink, formItems[2]);

  ScAddr const & instanceClassNode = m_memoryCtx.HelperFindBySystemIdtf(formItems[3]);

  if (formItems.size() > 4)
  {
    std::vector<std::string> relationsWithEntities = split(formItems[4], ", ");


    for (size_t i = 0; i < relationsWithEntities.size(); ++i)
    {
      std::vector<std::string> entityAndRelation = split(relationsWithEntities[i], " - ");

      ScAddr const & entity = m_memoryCtx.HelperFindBySystemIdtf(entityAndRelation[0]);
      ScAddr const & relation = m_memoryCtx.HelperFindBySystemIdtf(entityAndRelation[1]);

      instanceConstruction.TripleWithRelation(
        instanceNode,
        ScType::EdgeDCommonVar,
        entity,
        ScType::EdgeAccessVarPosPerm,
        relation
      );
    } 
  }

  instanceConstruction.Triple(
    InterfaceKeynodes::concept_wit_entity,
    ScType::EdgeAccessVarPosPerm,
    instanceNode
  );
  instanceConstruction.Triple(
    instanceClassNode,
    ScType::EdgeAccessVarPosPerm,
    instanceNode
  );
  instanceConstruction.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    mainIdtf
  );
  instanceConstruction.Triple(
    InterfaceKeynodes::lang_ru,
    ScType::EdgeAccessVarPosPerm,
    noteLink
  );
  instanceConstruction.TripleWithRelation(
    instanceNode,
    ScType::EdgeDCommonVar,
    mainIdtf,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_main_idtf
  );
  instanceConstruction.TripleWithRelation(
    instanceNode,
    ScType::EdgeDCommonVar,
    noteLink,
    ScType::EdgeAccessVarPosPerm,
    InterfaceKeynodes::nrel_note
  );

  ScTemplateGenResult full_construction;
  m_memoryCtx.HelperGenTemplate(instanceConstruction, full_construction);

  createAnswerMessageAndStructure("concept_phrase_about_success_creating_class_instance", answerStructure);

  for (size_t i = 0; i < full_construction.Size(); ++i)
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, answerStructure, full_construction[i]);
  }

  SC_LOG_DEBUG("CreateClassInstanceAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
  return SC_RESULT_OK;
}



std::vector<std::string> CreateClassInstanceAgent::split(const string & s, const string & delimiter)
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

bool CreateClassInstanceAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      InterfaceKeynodes::action_create_class_instance, actionAddr, ScType::EdgeAccessConstPosPerm);
}

void CreateClassInstanceAgent::createAnswer(std::string message)
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

void CreateClassInstanceAgent::createAnswerMessageAndStructure(std::string conceptName, ScAddr const & answerStructure)
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