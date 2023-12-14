#include <iostream>
#include <string>
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "keynodes/TestKeynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "keynodes/DialogKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "generator/MessageConstructionsGenerator.hpp"
#include "handler/LinkHandler.hpp"

#include "TestingAgent.hpp"

using namespace testModule;
using namespace scAgentsCommon;
using namespace commonModule;
using namespace dialogControlModule;

SC_AGENT_IMPLEMENTATION(TestingAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("TestingAgent started");

  
  
  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  
  if (!messageAddr.IsValid())
  {
     SC_LOG_ERROR("Message Addr not found.");
     utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
     SC_LOG_DEBUG("TestingAgent finished");
     return SC_RESULT_ERROR;
  }
  
  dialogControlModule::MessageConstructionsGenerator messageConstructionGenerator = MessageConstructionsGenerator(&m_memoryCtx);
  auto LinkHandler = commonModule:: LinkHandler(&m_memoryCtx); 
  
  ScAddr const & dialog = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);
  
  ScTemplate findMessageTextLink;
  
  findMessageTextLink.TripleWithRelation(
    ScType::NodeVar >> "_tuple",
    ScType::EdgeDCommonVar,
    messageAddr,
    ScType::EdgeAccessVarPosPerm,
    DialogKeynodes::nrel_sc_text_translation
  );
  
  findMessageTextLink.Triple(
    "_tuple",
    ScType::EdgeAccessVarPosPerm,
    ScType::LinkVar >> "_message_text_link"
  );
  
  ScTemplateSearchResult messageTextLink;
  m_memoryCtx.HelperSearchTemplate(findMessageTextLink, messageTextLink);
  
  std::string messageText = utils::CommonUtils::getLinkContent(&m_memoryCtx, messageTextLink[0]["_message_text_link"]);
  
  if (m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_success_authorization_status, dialog, ScType::EdgeAccessConstPosPerm) && messageText.find("Пройти тест по ТПИС") == 0)
  {
    SC_LOG_ERROR("zashol");
    ScAddr const &countOfQuestionsLinkFromMessage = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::count);
    ScAddr const &countOfQuestionsLinkConstruction = m_memoryCtx.CreateNode(ScType::NodeConst);
    std::string countOfQuestions = utils::CommonUtils::getLinkContent(&m_memoryCtx, countOfQuestionsLinkFromMessage);
    messageConstructionGenerator.generateTextTranslationConstruction(countOfQuestionsLinkConstruction, Keynodes::lang_ru, std::to_string(std::atoi(countOfQuestions.c_str()) - 1));
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, dialog, countOfQuestionsLinkConstruction, TestKeynodes::rrel_count_of_questions);

    

    ScAddr user = m_memoryCtx.CreateNode(ScType::NodeVar); 
    ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    if (it3->Next())
    {
      user = it3->Get(2);
      SC_LOG_ERROR("nashel");
    }
    

    ScAddr const &totalCountOfQuestions = m_memoryCtx.CreateNode(ScType::NodeConst);
    ScAddr const &countOfCorrectQuestions = m_memoryCtx.CreateNode(ScType::NodeConst);
    messageConstructionGenerator.generateTextTranslationConstruction(totalCountOfQuestions, Keynodes::lang_ru, countOfQuestions);
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, user, totalCountOfQuestions, TestKeynodes::rrel_total);
    messageConstructionGenerator.generateTextTranslationConstruction(countOfCorrectQuestions, Keynodes::lang_ru, "0");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, user, countOfCorrectQuestions, TestKeynodes::rrel_correct);


    ScTemplate searchQuestionFormulationLink;
    searchQuestionFormulationLink.Triple(
      TestKeynodes::concept_questions,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_questionNode"
    );
    searchQuestionFormulationLink.TripleWithRelation(
      "_questionNode",
      ScType::EdgeAccessVarPosPerm,
      ScType::LinkVar >> "_questionLink",
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::rrel_question_formulation
    );
    ScTemplateSearchResult getQuestionFormulationLink;
    std::string questionFormulationText;
    m_memoryCtx.HelperSearchTemplate(searchQuestionFormulationLink, getQuestionFormulationLink);
    if(getQuestionFormulationLink.IsEmpty() == SC_FALSE)
    {
      SC_LOG_ERROR("tozhe_zashel");
      questionFormulationText = utils::CommonUtils::getLinkContent(&m_memoryCtx, getQuestionFormulationLink[std::atoi(countOfQuestions.c_str()) - 1]["_questionLink"]);
      SC_LOG_ERROR(questionFormulationText);
    }
    SC_LOG_ERROR(questionFormulationText);
    ScAddr const &replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
    messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, questionFormulationText);
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
    
  }
  else if (m_memoryCtx.HelperCheckEdge(TestKeynodes::concept_success_authorization_status, dialog, ScType::EdgeAccessConstPosPerm) && messageText.find("Ответ:") == 0)
  {
    ScAddr user = m_memoryCtx.CreateNode(ScType::NodeVar); 
    ScIterator3Ptr it3 = m_memoryCtx.Iterator3(TestKeynodes::concept_authorized_user, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    if (it3->Next())
    {
      user = it3->Get(2);
      SC_LOG_ERROR("1");
    }
    SC_LOG_ERROR("2");
    ScAddr const &replyAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    SC_LOG_ERROR("3");
    ScTemplate searchCountOfQuestions;
    SC_LOG_ERROR("4");
    searchCountOfQuestions.TripleWithRelation(
      dialog,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_questionCountConstruction",
      ScType::EdgeAccessVarPosPerm,
      TestKeynodes::rrel_count_of_questions
    );
    searchCountOfQuestions.TripleWithRelation(
      ScType::NodeVar >> "_tuple",
      ScType::EdgeDCommonVar,
      "_questionCountConstruction",
      ScType::EdgeAccessVarPosPerm,
      DialogKeynodes::nrel_sc_text_translation
    );
    searchCountOfQuestions.Triple(
      "_tuple",
      ScType::EdgeAccessVarPosPerm,
      ScType::LinkVar >> "_questionCountLink"
    );
    SC_LOG_ERROR("5");
    ScTemplateSearchResult getCountOfQuestions;
    SC_LOG_ERROR("6");
    std::string countOfQuestions;
    SC_LOG_ERROR("7");
    m_memoryCtx.HelperSearchTemplate(searchCountOfQuestions, getCountOfQuestions);
    SC_LOG_ERROR("8");
    if(getCountOfQuestions.IsEmpty() == SC_FALSE)
    {
      SC_LOG_ERROR("9");
      countOfQuestions = utils::CommonUtils::getLinkContent(&m_memoryCtx, getCountOfQuestions[0]["_questionCountLink"]);
      SC_LOG_ERROR("10");
      m_memoryCtx.EraseElement(getCountOfQuestions[0]["_questionCountConstruction"]);
    }
    SC_LOG_ERROR("11");
    ScAddr const &countOfQuestionsLinkConstruction = m_memoryCtx.CreateNode(ScType::NodeConst);
    SC_LOG_ERROR("12");
    messageConstructionGenerator.generateTextTranslationConstruction(countOfQuestionsLinkConstruction, Keynodes::lang_ru, std::to_string(std::atoi(countOfQuestions.c_str()) - 1));
    SC_LOG_ERROR("13");
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, dialog, countOfQuestionsLinkConstruction, TestKeynodes::rrel_count_of_questions);
    SC_LOG_ERROR("14");

    if(std::atoi(countOfQuestions.c_str()) - 1 != -1)
    {
      SC_LOG_ERROR("15");
      ScAddr const &userAnswer = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::answer);
      SC_LOG_ERROR("16");
      ScTemplate searchCorrectAnswer;
      SC_LOG_ERROR("17");
      searchCorrectAnswer.TripleWithRelation(
        ScType::NodeVar >> "_questionConstruction",
        ScType::EdgeAccessVarPosPerm,
        ScType::LinkVar >> "_linkCorrectAnswer",
        ScType::EdgeAccessVarPosPerm,
        TestKeynodes::rrel_answer
      );
      searchCorrectAnswer.Triple(
        TestKeynodes::concept_correct_answer,
        ScType::EdgeAccessVarPosPerm,
        "_linkCorrectAnswer"
      );
      ScTemplateSearchResult getCorrectAnswer;
      SC_LOG_ERROR("18");
      m_memoryCtx.HelperSearchTemplate(searchCorrectAnswer, getCorrectAnswer);
      SC_LOG_ERROR("19");
      std::string correctAnswer = utils::CommonUtils::getLinkContent(&m_memoryCtx, getCorrectAnswer[std::atoi(countOfQuestions.c_str())]["_linkCorrectAnswer"]);
      SC_LOG_ERROR("20");
      ScTemplate searchQuestionFormulationLink;
      SC_LOG_ERROR("21");
      searchQuestionFormulationLink.Triple(
        TestKeynodes::concept_questions,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_questionNode"
      );
      searchQuestionFormulationLink.TripleWithRelation(
        "_questionNode",
        ScType::EdgeAccessVarPosPerm,
        ScType::LinkVar >> "_questionLink",
        ScType::EdgeAccessVarPosPerm,
        TestKeynodes::rrel_question_formulation
      );
      ScTemplateSearchResult getQuestionFormulationLink;
      SC_LOG_ERROR("22");
      std::string questionFormulationText;
      SC_LOG_ERROR("23");
      m_memoryCtx.HelperSearchTemplate(searchQuestionFormulationLink, getQuestionFormulationLink);
      SC_LOG_ERROR("24");
      if(getQuestionFormulationLink.IsEmpty() == SC_FALSE)
      {
        SC_LOG_ERROR("25");
        questionFormulationText = utils::CommonUtils::getLinkContent(&m_memoryCtx, getQuestionFormulationLink[std::atoi(countOfQuestions.c_str()) - 1]["_questionLink"]);
      }
      SC_LOG_ERROR("26");
      if(utils::CommonUtils::getLinkContent(&m_memoryCtx, userAnswer) == correctAnswer)
      {
        SC_LOG_ERROR("27");
        m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
        SC_LOG_ERROR("28");
        messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Верно!<br><br>" + questionFormulationText);
        SC_LOG_ERROR("Верно!<br><br>" + questionFormulationText);
        utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
        SC_LOG_ERROR("30");
        ScTemplate searchCountOfCorrectAnswers;
        SC_LOG_ERROR("31");
        SC_LOG_ERROR(user.IsValid());
        searchCountOfCorrectAnswers.TripleWithRelation(
          user,
          ScType::EdgeAccessVarPosPerm,
          ScType::NodeVar >> "_correctAnswerConstruction",
          ScType::EdgeAccessVarPosPerm,
          TestKeynodes::rrel_correct
        );
        SC_LOG_ERROR("91");
        searchCountOfCorrectAnswers.TripleWithRelation(
          ScType::NodeVar >> "_tuple",
          ScType::EdgeDCommonVar,
          "_correctAnswerConstruction",
          ScType::EdgeAccessVarPosPerm,
          DialogKeynodes::nrel_sc_text_translation
        );
        SC_LOG_ERROR("92");
        searchCountOfCorrectAnswers.Triple(
          "_tuple",
          ScType::EdgeAccessVarPosPerm,
          ScType::LinkVar >> "_countOfCorrectAnswersLink"
        );
        SC_LOG_ERROR("93");
        ScTemplateSearchResult getCountOfCorrectAnswers;
        SC_LOG_ERROR("32");
        m_memoryCtx.HelperSearchTemplate(searchCountOfCorrectAnswers, getCountOfCorrectAnswers);
        SC_LOG_ERROR("33");
        std::string countOfCorrectAnswers = utils::CommonUtils::getLinkContent(&m_memoryCtx, getCountOfCorrectAnswers[0]["_countOfCorrectAnswersLink"]);
        SC_LOG_ERROR("34");
        m_memoryCtx.EraseElement(utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, user, TestKeynodes::rrel_correct));
        SC_LOG_ERROR("35");
        ScAddr const &newCorrectAnswersConstruction = m_memoryCtx.CreateNode(ScType::NodeConst);
        SC_LOG_ERROR("36");
        messageConstructionGenerator.generateTextTranslationConstruction(newCorrectAnswersConstruction, Keynodes::lang_ru, std::to_string(std::atoi(countOfCorrectAnswers.c_str()) + 1));
        SC_LOG_ERROR("37");
        utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, user, newCorrectAnswersConstruction, TestKeynodes::rrel_correct);
        SC_LOG_ERROR("38");
      }
      else
      {
        SC_LOG_ERROR("39");
        m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
        SC_LOG_ERROR("40");
        messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Неверно. Правильный ответ " + correctAnswer + ".<br><br>" + questionFormulationText);
        SC_LOG_ERROR("Неверно. Правильный ответ " + correctAnswer + ".<br><br>" + questionFormulationText);
        utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
      }
    }
    else
    {
      ScAddr const &userAnswer = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, TestKeynodes::answer);

      ScTemplate searchCorrectAnswer;
      searchCorrectAnswer.TripleWithRelation(
        ScType::NodeVar >> "_questionConstruction",
        ScType::EdgeAccessVarPosPerm,
        ScType::LinkVar >> "_linkCorrectAnswer",
        ScType::EdgeAccessVarPosPerm,
        TestKeynodes::rrel_answer
      );
      searchCorrectAnswer.Triple(
        TestKeynodes::concept_correct_answer,
        ScType::EdgeAccessVarPosPerm,
        "_linkCorrectAnswer"
      );
      ScTemplateSearchResult getCorrectAnswer;
      m_memoryCtx.HelperSearchTemplate(searchCorrectAnswer, getCorrectAnswer);
      std::string correctAnswer = utils::CommonUtils::getLinkContent(&m_memoryCtx, getCorrectAnswer[std::atoi(countOfQuestions.c_str())]["_linkCorrectAnswer"]);

      ScTemplate searchTotalAndCorrectAnswersCounts;
      searchTotalAndCorrectAnswersCounts.Triple(
        TestKeynodes::concept_authorized_user,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "user"
      );
      searchTotalAndCorrectAnswersCounts.TripleWithRelation(
        "user",
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_correctAnswersConstruction",
        ScType::EdgeAccessVarPosPerm,
        TestKeynodes::rrel_correct
      );
      searchTotalAndCorrectAnswersCounts.TripleWithRelation(
        ScType::NodeVar >> "_tuple_1",
        ScType::EdgeDCommonVar,
        "_correctAnswersConstruction",
        ScType::EdgeAccessVarPosPerm,
        DialogKeynodes::nrel_sc_text_translation
      );
      searchTotalAndCorrectAnswersCounts.Triple(
        "_tuple_1",
        ScType::EdgeAccessVarPosPerm,
        ScType::LinkVar >> "_correctAnswersLink"
      );
      searchTotalAndCorrectAnswersCounts.TripleWithRelation(
        "user",
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_totalAnswersConstruction",
        ScType::EdgeAccessVarPosPerm,
        TestKeynodes::rrel_total
      );
      searchTotalAndCorrectAnswersCounts.TripleWithRelation(
        ScType::NodeVar >> "_tuple_2",
        ScType::EdgeDCommonVar,
        "_totalAnswersConstruction",
        ScType::EdgeAccessVarPosPerm,
        DialogKeynodes::nrel_sc_text_translation
      );
      searchTotalAndCorrectAnswersCounts.Triple(
        "_tuple_2",
        ScType::EdgeAccessVarPosPerm,
        ScType::LinkVar >> "_totalAnswersLink"
      );
      ScTemplateSearchResult getTotalAndCorrectAnswersCounts;
      m_memoryCtx.HelperSearchTemplate(searchTotalAndCorrectAnswersCounts, getTotalAndCorrectAnswersCounts);
      std::string countOfCorrectAnswers = utils::CommonUtils::getLinkContent(&m_memoryCtx, getTotalAndCorrectAnswersCounts[0]["_correctAnswersLink"]);
      std::string totalCountOfAnswers = utils::CommonUtils::getLinkContent(&m_memoryCtx, getTotalAndCorrectAnswersCounts[0]["_totalAnswersLink"]);

      if(utils::CommonUtils::getLinkContent(&m_memoryCtx, userAnswer) == correctAnswer)
      {
        m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
        messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Верно!<br><br>Отчёт по пройденному тесту:<br>" + countOfCorrectAnswers + " правильных ответов из " + totalCountOfAnswers + ".");
        SC_LOG_ERROR("Верно!<br><br>Отчёт по пройденному тесту:<br>" + countOfCorrectAnswers + "правильных ответов из " + totalCountOfAnswers + ".");
        utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);

        ScTemplate searchCountOfCorrectAnswers;
        searchCountOfCorrectAnswers.TripleWithRelation(
          user,
          ScType::EdgeAccessVarPosPerm,
          ScType::NodeVar >> "_correctAnswerConstruction",
          ScType::EdgeAccessVarPosPerm,
          TestKeynodes::rrel_correct
        );
        searchCountOfCorrectAnswers.TripleWithRelation(
          ScType::NodeVar >> "_tuple",
          ScType::EdgeDCommonVar,
          "_correctAnswerConstruction",
          ScType::EdgeAccessVarPosPerm,
          DialogKeynodes::nrel_sc_text_translation
        );
        searchCountOfCorrectAnswers.Triple(
          "_tuple",
          ScType::EdgeAccessVarPosPerm,
          ScType::LinkVar >> "_countOfCorrectAnswersLink"
        );
        ScTemplateSearchResult getCountOfCorrectAnswers;
        m_memoryCtx.HelperSearchTemplate(searchCountOfCorrectAnswers, getCountOfCorrectAnswers);
        std::string countOfCorrectAnswers = utils::CommonUtils::getLinkContent(&m_memoryCtx, getCountOfCorrectAnswers[0]["_countOfCorrectAnswersLink"]);
        m_memoryCtx.EraseElement(utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, user, TestKeynodes::rrel_correct));
        ScAddr const &newCorrectAnswersConstruction = m_memoryCtx.CreateNode(ScType::NodeConst);
        messageConstructionGenerator.generateTextTranslationConstruction(newCorrectAnswersConstruction, Keynodes::lang_ru, std::to_string(std::atoi(countOfCorrectAnswers.c_str()) + 1));
        utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, user, newCorrectAnswersConstruction, TestKeynodes::rrel_correct);
      }
      else
      {
        m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_message, replyAddr);
        messageConstructionGenerator.generateTextTranslationConstruction(replyAddr, Keynodes::lang_ru, "Неверно. Правильный ответ " + correctAnswer + ".<br><br>Отчёт по пройденному тесту:<br>" + countOfCorrectAnswers + "правильных ответов из " + totalCountOfAnswers + ".");
        SC_LOG_ERROR("Неверно. Правильный ответ " + correctAnswer + ".<br><br>Отчёт по пройденному тесту:<br>" + countOfCorrectAnswers + "правильных ответов из " + totalCountOfAnswers + ".");
        utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, messageAddr, replyAddr, MessageKeynodes::nrel_reply);
      }
      
      ScIterator5Ptr const it5_deleteTemporaryNodes = m_memoryCtx.Iterator5(user, ScType::EdgeAccessConstPosPerm, ScType::LinkConst, ScType::EdgeAccessConstPosPerm, TestKeynodes::rrel_correct);
      while(it5_deleteTemporaryNodes->Next())
      {
        m_memoryCtx.EraseElement(it5_deleteTemporaryNodes->Get(1));
        m_memoryCtx.EraseElement(it5_deleteTemporaryNodes->Get(2));
        m_memoryCtx.EraseElement(it5_deleteTemporaryNodes->Get(3));
        m_memoryCtx.EraseElement(it5_deleteTemporaryNodes->Get(4));
      }
    }
  }
  
  SC_LOG_DEBUG("TestingAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
  return SC_RESULT_OK;
}

ScAddrVector TestingAgent::getAnswer(ScAddr const & user)
{
  ScTemplate answerTemplate;
  answerTemplate.Triple(
    TestKeynodes::concept_authorized_user,
    ScType::EdgeAccessVarPosPerm >> "_edge1",
    user
  );
  answerTemplate.Triple(
    TestKeynodes::concept_users,
    ScType::EdgeAccessVarPosPerm >> "_edge2",
    user
  );
  answerTemplate.TripleWithRelation(
    user,
    ScType::EdgeDCommonVar >> "_edge3",
    ScType::LinkVar >> "_login",
    ScType::EdgeAccessVarPosPerm >> "_edge4",
    TestKeynodes::nrel_login
  );
  answerTemplate.TripleWithRelation(
    user,
    ScType::EdgeDCommonVar >> "_edge5",
    ScType::LinkVar >> "_password",
    ScType::EdgeAccessVarPosPerm >> "_edge6",
    TestKeynodes::nrel_password
  );
  ScTemplateSearchResult answer;
  m_memoryCtx.HelperSearchTemplate(answerTemplate, answer);
  if (answer.IsEmpty() == SC_FALSE)
    return {answer[0]["_edge1"],answer[0]["_edge2"],answer[0]["_edge3"],answer[0]["_edge4"],
    answer[0]["_edge5"],answer[0]["_edge6"],answer[0]["_login"],
    answer[0]["_password"],user,
    TestKeynodes::concept_authorized_user, TestKeynodes::concept_users, TestKeynodes::nrel_login, 
    TestKeynodes::nrel_password};
  return {};                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
}

ScAddr TestingAgent::getLinkConstructionLogin(std::string const & text)
    {
      ScAddrVector const & possibleLinks = m_memoryCtx.FindLinksByContent(text); 
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

bool TestingAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      TestKeynodes::action_testing, actionAddr, ScType::EdgeAccessConstPosPerm);
}
