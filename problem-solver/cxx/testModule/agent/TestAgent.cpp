#include <fstream>
#include <iostream>
#include <string>
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "keynodes/TestKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "keynodes/DialogKeynodes.hpp"
#include "generator/MessageConstructionsGenerator.hpp"
#include "TestAgent.hpp"
#include "handler/LinkHandler.hpp"

using namespace testModule;
using namespace commonModule;
using namespace scAgentsCommon;
using namespace dialogControlModule;

SC_AGENT_IMPLEMENTATION(TestAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("TestAgent started");

    ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("TestAgent finished");
     return SC_RESULT_ERROR;
  }

   dialogControlModule::MessageConstructionsGenerator messageConstructionGenerator = MessageConstructionsGenerator(&m_memoryCtx);
   auto LinkHandler = commonModule:: LinkHandler(&m_memoryCtx); 
  //std::unique_ptr<dialogControlModule::MessageConstructionsGenerator> messageConstructionGenerator = std::make_unique<dialogControlModule::MessageConstructionsGenerator>;
  ScAddr const & dialog = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);

  if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_authorization_status, dialog, ScType::EdgeAccessConstPosPerm))
  {
    ScAddr const &count_quest = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::count);
   

    ScAddr const &link = m_memoryCtx.CreateNode(ScType::NodeConst);
    
    messageConstructionGenerator.generateTextTranslationConstruction(link, Keynodes::lang_ru, utils::CommonUtils::getLinkContent(&m_memoryCtx, count_quest));

    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, dialog, link, TestKeynodes::rrel_count_of_questions);

    SC_LOG_ERROR("help");
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Введите логин: ");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    //1. Используем ScIterator3Ptr. С помощью итератора находите дугу. Есди итератор  it3->Next(), то удаляете дугу.
    //2. m_memoryCtx.EraseElement(it3.Get(1));
    ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_authorization_status, ScType::EdgeAccessConstPosPerm, dialog);
    if (it3->Next())
      m_memoryCtx.EraseElement(it3->Get(1));
    
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_login_waiting_status, dialog);
  }

  else if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_login_waiting_status, dialog, ScType::EdgeAccessConstPosPerm))
  {
    ScAddr const &link_login = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::email);
    //добавить хэдер
    std::string login_from_message = utils::CommonUtils::getLinkContent(&m_memoryCtx, link_login);
    SC_LOG_ERROR(login_from_message);
    
    SC_LOG_ERROR("1");
    ScAddr const &user = getLinkConstructionLogin(login_from_message);
    SC_LOG_ERROR("2");
    if(user.IsValid())
    {
      SC_LOG_ERROR("3");
      
      ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_login_waiting_status, ScType::EdgeAccessConstPosPerm, dialog);
      SC_LOG_ERROR("4");
      if (it3->Next())
        m_memoryCtx.EraseElement(it3->Get(1));
      SC_LOG_ERROR("5");
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_password_waiting_status, dialog);
      SC_LOG_ERROR("6");
      ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
      SC_LOG_ERROR("7");
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
      SC_LOG_ERROR("8");
      //проверить везде
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Введите пароль: ");
      SC_LOG_ERROR("9");
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
      SC_LOG_ERROR("10");

      
    }
    else
    {
      ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_login_waiting_status, ScType::EdgeAccessConstPosPerm, dialog);
      if (it3->Next())
        m_memoryCtx.EraseElement(it3->Get(1));
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_login_not_found_status, dialog);
      ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Логин не найден. Пройдите регистрацию.<br>Введите логин и пароль:");
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    }
  }
  else if (m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_login_not_found_status, dialog, ScType::EdgeAccessConstPosPerm))
  {
    SC_LOG_ERROR("1");
    ScAddr const &link_login = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::email);
    SC_LOG_ERROR("2");
    ScAddr const &link_password = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::password);
    SC_LOG_ERROR("3");
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    SC_LOG_ERROR("4");
    ScTemplate createTemplate;
    SC_LOG_ERROR("5");
    createTemplate.Triple(
      TestKeynodes::concept_users,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_user"
    );
    SC_LOG_ERROR("6");
    createTemplate.TripleWithRelation(
      "_user",
      ScType::EdgeDCommonVar,
      LinkHandler.createLink(utils::CommonUtils::getLinkContent(&m_memoryCtx, link_login)),
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::nrel_login
    );
    SC_LOG_ERROR("7");
    createTemplate.TripleWithRelation(
      "_user",
      ScType::EdgeDCommonVar,
      LinkHandler.createLink(utils::CommonUtils::getLinkContent(&m_memoryCtx, link_password)),
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::nrel_password
    );
    SC_LOG_ERROR("8");
    ScTemplateGenResult templateGenResult;
    SC_LOG_ERROR("9");
    m_memoryCtx.HelperGenTemplate(createTemplate, templateGenResult);
    SC_LOG_ERROR("10");
    ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_login_not_found_status, ScType::EdgeAccessConstPosPerm, dialog);
    SC_LOG_ERROR("11");
    if(it3->Next())
      m_memoryCtx.EraseElement(it3->Get(1));
    SC_LOG_ERROR("12");
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_success_authorization_status, dialog);
    SC_LOG_ERROR("13");
    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Авторизация успешна. Приступить к тестированию?");
    SC_LOG_ERROR("14");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    SC_LOG_ERROR("15");
    ScAddr const &user = getLinkConstructionLogin(utils::CommonUtils::getLinkContent(&m_memoryCtx, link_login));
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_authorized_user, user);
    
  }

  else if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_password_waiting_status, dialog, ScType::EdgeAccessConstPosPerm))
  {
    ScAddr const &link_password = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::password);
    std::string password_from_message = utils::CommonUtils::getLinkContent(&m_memoryCtx, link_password);
    ScAddr const &user = getLinkConstructionPassword(password_from_message);
    if (user.IsValid())
    {
      ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_password_waiting_status, ScType::EdgeAccessConstPosPerm, dialog);
      if (it3->Next())
        m_memoryCtx.EraseElement(it3->Get(1));
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_success_authorization_status, dialog);
      ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Авторизация успешна. Приступить к тестированию?");
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_authorized_user, user);
    }
    else{
      ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Неверный пароль. Введите пароль ещё раз: ");
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    }
  }
  else if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_success_authorization_status, dialog, ScType::EdgeAccessConstPosPerm))
  {
      ScAddr user = m_memoryCtx.CreateNode(ScType::NodeVar); 
      ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
      if (it3->Next())
        user = it3->Get(2);
      
      //ScAddr const &number_of_questions = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, dialog, TestKeynodes::rrel_count_of_questions);
      //std::string num_of_quest = utils::CommonUtils::getLinkContent(&m_memoryCtx, number_of_questions);
      ScTemplate getLink;
      getLink.TripleWithRelation(
        dialog,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_node",
        ScType::EdgeAccessVarPosPerm,
        TestKeynodes::rrel_count_of_questions
      );
      getLink.TripleWithRelation(
        ScType::NodeVar >> "_node_2",
        ScType::EdgeDCommonVar,
        "_node",
        ScType::EdgeAccessVarPosPerm,
        DialogKeynodes::nrel_sc_text_translation
      );
      getLink.Triple(
        "_node_2",
        ScType::EdgeAccessVarPosPerm,
        ScType::LinkVar >> "_link"
      );
      ScTemplateSearchResult getCount;
      std::string num_of_quest;
      m_memoryCtx.HelperSearchTemplate(getLink, getCount);
      if (getLink.IsEmpty() == SC_FALSE)
      {
        num_of_quest = utils::CommonUtils::getLinkContent(&m_memoryCtx, getCount[0]["_link"]);
        SC_LOG_ERROR(num_of_quest);
      }
    
    m_memoryCtx.EraseElement(getCount[0]["_node"]);

      ScAddr const &total_count = m_memoryCtx.CreateNode(ScType::NodeConst);
      ScAddr const &correct_count = m_memoryCtx.CreateNode(ScType::NodeConst);
      messageConstructionGenerator.generateTextTranslationConstruction(total_count, Keynodes::lang_ru, num_of_quest);
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, user, total_count, TestKeynodes::rrel_total);
      messageConstructionGenerator.generateTextTranslationConstruction(correct_count, Keynodes::lang_ru, "0");
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, user, correct_count, TestKeynodes::rrel_correct);
      //m_memoryCtx.EraseElement(number_of_questions);
      ScAddr const &link = m_memoryCtx.CreateNode(ScType::NodeConst);
      SC_LOG_ERROR("1");
      messageConstructionGenerator.generateTextTranslationConstruction(link, Keynodes::lang_ru, std::to_string(std::atoi(num_of_quest.c_str()) - 1));
      SC_LOG_ERROR("num" + num_of_quest);
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, dialog, link, TestKeynodes::rrel_count_of_questions);
      SC_LOG_ERROR("1");
      std::string question_form;
      ScTemplate getQuestion;
      getQuestion.Triple(
        TestKeynodes::concept_questions,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_question"
      );
      getQuestion.TripleWithRelation(
        "_question",
        ScType::EdgeAccessVarPosPerm,
        ScType::LinkVar >> "_link",
        ScType::EdgeAccessVarPosPerm,
        TestKeynodes::rrel_question_formulation
      );
      ScTemplateSearchResult searchResult;
      m_memoryCtx.HelperSearchTemplate(getQuestion, searchResult);
      if(searchResult.IsEmpty() == SC_FALSE)
        question_form = utils::CommonUtils::getLinkContent(&m_memoryCtx, searchResult[std::atoi(num_of_quest.c_str()) - 1]["_link"]);
      ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    

      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, question_form);
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_current_question, searchResult[0]["_question"]);
      SC_LOG_ERROR("1");
      ScIterator3Ptr it3_2 = m_memoryCtx.Iterator3(TestKeynodes::concept_success_authorization_status, ScType::EdgeAccessConstPosPerm, dialog);
      SC_LOG_ERROR("2");
      if (it3_2->Next())
        m_memoryCtx.EraseElement(it3_2->Get(1));
      SC_LOG_ERROR("3");
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_waiting_answer, dialog);
      SC_LOG_ERROR("4");
  }
  else if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_waiting_answer, dialog, ScType::EdgeAccessConstPosPerm))
  {
    ScAddr user = m_memoryCtx.CreateNode(ScType::NodeConst);
    ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
      if (it3->Next())
        user = it3->Get(2);
   // ScAddr const &number_of_question = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, dialog, TestKeynodes::rrel_count_of_questions);
    SC_LOG_ERROR("4");
    //ScIterator3Ptr it3_2 = m_memoryCtx.Iterator3(TestKeynodes::concept_current_question, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    SC_LOG_ERROR("2");
    //if (it3_2->Next())
     // m_memoryCtx.EraseElement(it3_2->Get(2));
    ScTemplate getLink;
      getLink.TripleWithRelation(
        dialog,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_node",
        ScType::EdgeAccessVarPosPerm,
        TestKeynodes::rrel_count_of_questions
      );
      getLink.TripleWithRelation(
        ScType::NodeVar >> "_node_2",
        ScType::EdgeDCommonVar,
        "_node",
        ScType::EdgeAccessVarPosPerm,
        DialogKeynodes::nrel_sc_text_translation
      );
      getLink.Triple(
        "_node_2",
        ScType::EdgeAccessVarPosPerm,
        ScType::LinkVar >> "_link"
      );
      ScTemplateSearchResult getCount;
      std::string num_of_quest;
      m_memoryCtx.HelperSearchTemplate(getLink, getCount);
      if (getLink.IsEmpty() == SC_FALSE)
      {
        num_of_quest = utils::CommonUtils::getLinkContent(&m_memoryCtx, getCount[0]["_link"]);
        SC_LOG_ERROR(num_of_quest);
      }
    
    m_memoryCtx.EraseElement(getCount[0]["_node"]);
    ScAddr const &link = m_memoryCtx.CreateNode(ScType::NodeConst);
    SC_LOG_ERROR("ffdg" + num_of_quest + "8");
    SC_LOG_ERROR("num" + num_of_quest);
    messageConstructionGenerator.generateTextTranslationConstruction(link, Keynodes::lang_ru, std::to_string(std::atoi(num_of_quest.c_str()) - 1));
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, dialog, link, TestKeynodes::rrel_count_of_questions);
   
    if(std::atoi(num_of_quest.c_str()) - 1 != -1)
    {
       ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    ScAddr const &answer = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::answer);
    ScTemplate getCorrectAnswer;
    getCorrectAnswer.TripleWithRelation(
      ScType::NodeVar >> "_question",
      ScType::EdgeAccessVarPosPerm,
      ScType::LinkVar >> "_link",
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::rrel_answer
    );
    getCorrectAnswer.Triple(
      TestKeynodes::concept_correct_answer,
      ScType::EdgeAccessVarPosPerm,
      "_link"
    );
    ScTemplateSearchResult searchResult;
    std::string correctAnswer;
    m_memoryCtx.HelperSearchTemplate(getCorrectAnswer, searchResult);
    if(searchResult.IsEmpty() == SC_FALSE)
      correctAnswer = utils::CommonUtils::getLinkContent(&m_memoryCtx, searchResult[std::atoi(num_of_quest.c_str())]["_link"]);

    ScTemplate getQuestion;
    getQuestion.Triple(
        TestKeynodes::concept_questions,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_question"
      );
    getQuestion.TripleWithRelation(
      "_question",
      ScType::EdgeAccessVarPosPerm,
      ScType::LinkVar >> "_link",
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::rrel_question_formulation
    );
    ScTemplateSearchResult search;
    std::string question;
    m_memoryCtx.HelperSearchTemplate(getQuestion, search);
    if(searchResult.IsEmpty() == SC_FALSE)
      question = utils::CommonUtils::getLinkContent(&m_memoryCtx, search[std::atoi(num_of_quest.c_str()) - 1]["_link"]);
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_current_question, search[1]["_link"]);
     


    if(utils::CommonUtils::getLinkContent(&m_memoryCtx, answer) == correctAnswer)
    {
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Верно.<br><br>" + question);
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
      ScTemplate getCorrectAns;
      getCorrectAns.TripleWithRelation(
        user,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_svyazka",
        ScType::EdgeAccessVarPosPerm,
        TestKeynodes::rrel_correct
      );
      getCorrectAns.TripleWithRelation(
        ScType::NodeVar >> "_tuple",
        ScType::EdgeDCommonVar,
        "_svyazka",
        ScType::EdgeAccessVarPosPerm,
        DialogKeynodes::nrel_sc_text_translation
      );
      getCorrectAns.Triple(
        "_tuple",
        ScType::EdgeAccessVarPosPerm,
        ScType::LinkVar >> "correct_anss"
      );
      ScTemplateSearchResult result;
      std::string correct_answers_old;
      m_memoryCtx.HelperSearchTemplate(getCorrectAns, result);
      if(result.IsEmpty() == SC_FALSE)
      {
        correct_answers_old = utils::CommonUtils::getLinkContent(&m_memoryCtx, result[0]["correct_anss"]);
      }
      m_memoryCtx.EraseElement(utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, user, TestKeynodes::rrel_correct));
      ScAddr const &new_correct = m_memoryCtx.CreateNode(ScType::NodeConst);
      messageConstructionGenerator.generateTextTranslationConstruction(new_correct, Keynodes::lang_ru, std::to_string(std::atoi(correct_answers_old.c_str()) + 1));
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, user, new_correct, TestKeynodes::rrel_correct);
    }
    else
    {
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Неверно. Правильный ответ " + correctAnswer + ".<br><br>" + question);
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    }
    }
    else
    {
      ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    ScAddr const &answer = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::answer);
    ScTemplate getCorrectAnswer;
    getCorrectAnswer.TripleWithRelation(
      ScType::NodeVar >> "_question",
      ScType::EdgeAccessVarPosPerm,
      ScType::LinkVar >> "_link",
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::rrel_answer
    );
    getCorrectAnswer.Triple(
      TestKeynodes::concept_correct_answer,
      ScType::EdgeAccessVarPosPerm,
      "_link"
    );
    ScTemplateSearchResult searchResult;
    std::string correctAnswer;
    m_memoryCtx.HelperSearchTemplate(getCorrectAnswer, searchResult);
    if(searchResult.IsEmpty() == SC_FALSE)
      correctAnswer = utils::CommonUtils::getLinkContent(&m_memoryCtx, searchResult[std::atoi(num_of_quest.c_str())]["_link"]);
    if(utils::CommonUtils::getLinkContent(&m_memoryCtx, answer) == correctAnswer)
    {
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Верно.");
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
      ScTemplate getCorrectAns;
      getCorrectAns.TripleWithRelation(
        user,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_svyazka",
        ScType::EdgeAccessVarPosPerm,
        TestKeynodes::rrel_correct
      );
      getCorrectAns.TripleWithRelation(
        ScType::NodeVar >> "_tuple",
        ScType::EdgeDCommonVar,
        "_svyazka",
        ScType::EdgeAccessVarPosPerm,
        DialogKeynodes::nrel_sc_text_translation
      );
      getCorrectAns.Triple(
        "_tuple",
        ScType::EdgeAccessVarPosPerm,
        ScType::LinkVar >> "correct_anss"
      );
      ScTemplateSearchResult result;
      std::string correct_answers_old;
      m_memoryCtx.HelperSearchTemplate(getCorrectAns, result);
      if(result.IsEmpty() == SC_FALSE)
      {
        correct_answers_old = utils::CommonUtils::getLinkContent(&m_memoryCtx, result[0]["correct_anss"]);
        m_memoryCtx.EraseElement(result[0]["_svyazka"]);
      }
      ScAddr const &new_correct = m_memoryCtx.CreateNode(ScType::NodeConst);
      messageConstructionGenerator.generateTextTranslationConstruction(new_correct, Keynodes::lang_ru, std::to_string(std::atoi(correct_answers_old.c_str()) + 1));
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, user, new_correct, TestKeynodes::rrel_correct);
    }
    else
    {
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
      messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Неверно. Правильный ответ " + correctAnswer + ".");
      utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    }
    }

    if(std::atoi(num_of_quest.c_str()) - 2 == -2)
    {
      ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_waiting_answer, ScType::EdgeAccessConstPosPerm, dialog);
      if (it3->Next())
        m_memoryCtx.EraseElement(it3->Get(1));
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, TestKeynodes::concept_status_finish, dialog);
    } 
  }
  else if(m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_status_finish, dialog, ScType::EdgeAccessConstPosPerm))
  {
    
    ScTemplate countsSearch;
    countsSearch.Triple(
      TestKeynodes::concept_authorized_user,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "user"
    );
    countsSearch.TripleWithRelation(
      "user",
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_svyazka_1",
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::rrel_correct
    );
    countsSearch.TripleWithRelation(
      ScType::NodeVar >> "_tuple_1",
      ScType::EdgeDCommonVar,
      "_svyazka_1",
      ScType::EdgeAccessVarPosPerm,
      DialogKeynodes::nrel_sc_text_translation
    );
    countsSearch.Triple(
      "_tuple_1",
      ScType::EdgeAccessVarPosPerm,
      ScType::LinkVar >> "correct_anss"
    );
    countsSearch.TripleWithRelation(
      "user",
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_svyazka_2",
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::rrel_total
    );
    countsSearch.TripleWithRelation(
      ScType::NodeVar >> "_tuple_2",
      ScType::EdgeDCommonVar,
      "_svyazka_2",
      ScType::EdgeAccessVarPosPerm,
      DialogKeynodes::nrel_sc_text_translation
    );
    countsSearch.Triple(
      "_tuple_2",
      ScType::EdgeAccessVarPosPerm,
      ScType::LinkVar >> "total_anss"
    );
    ScTemplateSearchResult result;
    std::string correct_answers;
    std::string total_answers;
    m_memoryCtx.HelperSearchTemplate(countsSearch, result);
    if(result.IsEmpty() == SC_FALSE)
    {
      correct_answers = utils::CommonUtils::getLinkContent(&m_memoryCtx, result[0]["correct_anss"]);
      total_answers = utils::CommonUtils::getLinkContent(&m_memoryCtx, result[0]["total_anss"]);
    }
    ScAddr const & replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Отчёт по пройденному тесту:<br>"+correct_answers+" правильных ответов из "+total_answers+".");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
  }
  SC_LOG_DEBUG("TestAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
  return SC_RESULT_OK;
}

ScAddr TestAgent::getLinkConstructionLogin(std::string const & text)
    {
      ScAddrVector const & possibleLinks = m_memoryCtx.FindLinksByContent(text); // ищем ссылки
      for (ScAddr const & possibleLink : possibleLinks)
      {
        ScTemplate scTemplate;
        scTemplate.Triple(
          TestKeynodes::concept_users,
          ScType::EdgeAccessVarPosPerm,
          ScType::NodeVar >> "_user"
        );
        scTemplate.TripleWithRelation(
          "_user",
          ScType::EdgeDCommonVar,
          possibleLink,
          ScType::EdgeAccessVarPosPerm,
          TestKeynodes::nrel_login
        );
        ScTemplateSearchResult searchResult;
        m_memoryCtx.HelperSearchTemplate(scTemplate, searchResult);
        if (searchResult.IsEmpty() == SC_FALSE)
          return searchResult[0]["_user"];
      }
      return ScAddr::Empty;
    }

ScAddr TestAgent::getLinkConstructionPassword(std::string const & text)
    {
      ScAddrVector const & possibleLinks = m_memoryCtx.FindLinksByContent(text); // ищем ссылки
      for (ScAddr const & possibleLink : possibleLinks)
      {
        ScTemplate scTemplate;
        scTemplate.Triple(
          TestKeynodes::concept_users,
          ScType::EdgeAccessVarPosPerm,
          ScType::NodeVar >> "_user"
        );
        scTemplate.TripleWithRelation(
          "_user",
          ScType::EdgeDCommonVar,
          possibleLink,
          ScType::EdgeAccessVarPosPerm,
          TestKeynodes::nrel_password
        );
        ScTemplateSearchResult searchResult;
        m_memoryCtx.HelperSearchTemplate(scTemplate, searchResult);
        if (searchResult.IsEmpty() == SC_FALSE)
          return searchResult[0]["_user"];
      }
      return ScAddr::Empty;
    }

/*std::string GetTextAnswer(ScAddr const &questionTpisNode)
{
  ScTemplate textQuestion;
  textQuestion.TripleWithRelation(
    ScType::NodeVar >> "_node",
    ScType::EdgeAccessVarPosPerm,
    questionTpisNode,
    ScType::EdgeAccessVarPosPerm,
    DialogKeynodes::rrel_key_sc_element
  );
  textQuestion.TripleWithRelation(
   ScType::NodeVar >> "_node_1",
   ScType::EdgeDCommonVar,
   "_node",
   ScType::EdgeAccessVarPosPerm,
   DialogKeynodes::nrel_sc_text_translation
  );
  textQuestion.TripleWithRelation(
   "_node_1",
   ScType::EdgeAccessVarPosPerm,
    ScType::LinkVar >> "link_text",
   ScType::EdgeAccessVarPosPerm,
   Keynodes::rrel_example
  );

  ScTemplateSearchResult result;
  ScAddr link;
  if (m_memoryCtx.HelperSearchTemplate(textQuestion, result))
     link = result[0]["link_text"];


  return utils::CommonUtils::getLinkContent(&m_memoryCtx, link);
}*/

bool TestAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      TestKeynodes::action_test_for_tpis, actionAddr, ScType::EdgeAccessConstPosPerm);
}
