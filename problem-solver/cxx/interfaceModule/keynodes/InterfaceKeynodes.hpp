#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "InterfaceKeynodes.generated.hpp"

namespace interfaceModule
{
class InterfaceKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_change_interface"), ForceCreate)
  static ScAddr action_change_interface;

  SC_PROPERTY(Keynode("action_create_question_class_and_phrase_template"), ForceCreate)
  static ScAddr action_create_question_class_and_phrase_template;

  SC_PROPERTY(Keynode("action_create_class"), ForceCreate)
  static ScAddr action_create_class;

  SC_PROPERTY(Keynode("concept_header"), ForceCreate)
  static ScAddr concept_header;

  SC_PROPERTY(Keynode("concept_main_part"), ForceCreate)
  static ScAddr concept_main_part;

  SC_PROPERTY(Keynode("concept_footer"), ForceCreate)
  static ScAddr concept_footer;

  SC_PROPERTY(Keynode("concept_phrase"), ForceCreate)
  static ScAddr concept_phrase;
  
  SC_PROPERTY(Keynode("concept_intent_possible_class"), ForceCreate)
  static ScAddr concept_intent_possible_class;

  SC_PROPERTY(Keynode("concept_message"), ForceCreate)
  static ScAddr concept_message;

  SC_PROPERTY(Keynode("concept_answer_on_standard_message_rule_priority_1"), ForceCreate)
  static ScAddr concept_answer_on_standard_message_rule_priority_1;

  SC_PROPERTY(Keynode("concept_atomic_message"), ForceCreate)
  static ScAddr concept_atomic_message;

  SC_PROPERTY(Keynode("nrel_component_color"), ForceCreate)
  static ScAddr nrel_component_color;

  SC_PROPERTY(Keynode("nrel_inclusion"), ForceCreate)
  static ScAddr nrel_inclusion;

  SC_PROPERTY(Keynode("nrel_main_idtf"), ForceCreate)
  static ScAddr nrel_main_idtf;

  SC_PROPERTY(Keynode("nrel_answer"), ForceCreate)
  static ScAddr nrel_answer;

  SC_PROPERTY(Keynode("nrel_wit_ai_idtf"), ForceCreate)
  static ScAddr nrel_wit_ai_idtf;
  
  SC_PROPERTY(Keynode("nrel_reply"), ForceCreate)
  static ScAddr nrel_reply;

  SC_PROPERTY(Keynode("nrel_implication"), ForceCreate)
  static ScAddr nrel_implication;

  SC_PROPERTY(Keynode("nrel_answer_pattern"), ForceCreate)
  static ScAddr nrel_answer_pattern;

  SC_PROPERTY(Keynode("rrel_entity"), ForceCreate)
  static ScAddr rrel_entity;

  SC_PROPERTY(Keynode("rrel_color"), ForceCreate)
  static ScAddr rrel_color;

  SC_PROPERTY(Keynode("rrel_main_key_sc_element"), ForceCreate)
  static ScAddr rrel_main_key_sc_element;

  SC_PROPERTY(Keynode("lang_ru"), ForceCreate)
  static ScAddr lang_ru;
  
  SC_PROPERTY(Keynode("lang_en"), ForceCreate)
  static ScAddr lang_en;

  SC_PROPERTY(Keynode("atomic_logical_formula"), ForceCreate)
  static ScAddr atomic_logical_formula;

  SC_PROPERTY(Keynode("_message"), ForceCreate)
  static ScAddr _message;

  SC_PROPERTY(Keynode("_reply_message"), ForceCreate)
  static ScAddr _reply_message;
};

}  // namespace interfaceModule
