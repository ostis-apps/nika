#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "TestKeynodes.generated.hpp"

namespace testModule
{
class TestKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_test_for_tpis"), ForceCreate)
  static ScAddr action_test_for_tpis;

  SC_PROPERTY(Keynode("action_authorization"), ForceCreate)
  static ScAddr action_authorization;

  SC_PROPERTY(Keynode("action_registration"), ForceCreate)
  static ScAddr action_registration;

  SC_PROPERTY(Keynode("action_testing"), ForceCreate)
  static ScAddr action_testing;

  SC_PROPERTY(Keynode("concept_authorization_status"), ForceCreate)
  static ScAddr concept_authorization_status;

  SC_PROPERTY(Keynode("concept_login_waiting_status"), ForceCreate)
  static ScAddr concept_login_waiting_status;

  SC_PROPERTY(Keynode("concept_password_waiting_status"), ForceCreate)
  static ScAddr concept_password_waiting_status;

  SC_PROPERTY(Keynode("concept_login_not_found_status"), ForceCreate)
  static ScAddr concept_login_not_found_status;

  SC_PROPERTY(Keynode("concept_success_authorization_status"), ForceCreate)
  static ScAddr concept_success_authorization_status;

  SC_PROPERTY(Keynode("concept_success_registration_status"), ForceCreate)
  static ScAddr concept_success_registration_status;

  SC_PROPERTY(Keynode("concept_waiting_answer"), ForceCreate)
  static ScAddr concept_waiting_answer;

  SC_PROPERTY(Keynode("nrel_login"), ForceCreate)
  static ScAddr nrel_login;

  SC_PROPERTY(Keynode("nrel_password"), ForceCreate)
  static ScAddr nrel_password;

  SC_PROPERTY(Keynode("lang_ru"), ForceCreate)
  static ScAddr lang_ru;

  SC_PROPERTY(Keynode("concept_users"), ForceCreate)
  static ScAddr concept_users;

  SC_PROPERTY(Keynode("nrel_decomposition"), ForceCreate)
  static ScAddr nrel_decomposition;

  SC_PROPERTY(Keynode("email"), ForceCreate)
  static ScAddr email;

  SC_PROPERTY(Keynode("password"), ForceCreate)
  static ScAddr password;

  SC_PROPERTY(Keynode("count"), ForceCreate)
  static ScAddr count;

  SC_PROPERTY(Keynode("concept_authorized_user"), ForceCreate)
  static ScAddr concept_authorized_user;

  SC_PROPERTY(Keynode("concept_questions"), ForceCreate)
  static ScAddr concept_questions;

  SC_PROPERTY(Keynode("rrel_question_formulation"), ForceCreate)
  static ScAddr rrel_question_formulation;

  SC_PROPERTY(Keynode("concept_current_question"), ForceCreate)
  static ScAddr concept_current_question;

  SC_PROPERTY(Keynode("answer"), ForceCreate)
  static ScAddr answer;
  
  SC_PROPERTY(Keynode("concept_correct_answer"), ForceCreate)
  static ScAddr concept_correct_answer;

  SC_PROPERTY(Keynode("rrel_answer"), ForceCreate)
  static ScAddr rrel_answer;

  SC_PROPERTY(Keynode("rrel_count_of_questions"), ForceCreate)
  static ScAddr rrel_count_of_questions;

  SC_PROPERTY(Keynode("concept_status_finish"), ForceCreate)
  static ScAddr concept_status_finish;

  SC_PROPERTY(Keynode("rrel_total"), ForceCreate)
  static ScAddr rrel_total;

  SC_PROPERTY(Keynode("rrel_correct"), ForceCreate)
  static ScAddr rrel_correct;

  SC_PROPERTY(Keynode("nrel_report"), ForceCreate)
  static ScAddr nrel_report;

  SC_PROPERTY(Keynode("concept_absence_of_authorized_user"), ForceCreate)
  static ScAddr concept_absence_of_authorized_user;
};

}  // namespace interfaceModule
