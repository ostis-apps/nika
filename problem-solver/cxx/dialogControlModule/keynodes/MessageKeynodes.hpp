#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "MessageKeynodes.generated.hpp"

namespace dialogControlModule
{
class MessageKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_phrase_generation"), ForceCreate)
  static ScAddr action_phrase_generation;

  SC_PROPERTY(Keynode("action_standard_message_reply"), ForceCreate)
  static ScAddr action_standard_message_reply;

  SC_PROPERTY(Keynode("action_add_message_to_the_dialog"), ForceCreate)
  static ScAddr action_add_message_to_the_dialog;

  SC_PROPERTY(Keynode("concept_answer_on_standard_message_rule"), ForceCreate)
  static ScAddr concept_answer_on_standard_message_rule;

  SC_PROPERTY(Keynode("concept_answer_on_standard_message_rule_class_by_priority"), ForceCreate)
  static ScAddr concept_answer_on_standard_message_rule_class_by_priority;

  SC_PROPERTY(Keynode("concept_atomic_message"), ForceCreate)
  static ScAddr concept_atomic_message;

  SC_PROPERTY(Keynode("concept_non_atomic_message"), ForceCreate)
  static ScAddr concept_non_atomic_message;

  SC_PROPERTY(Keynode("concept_message"), ForceCreate)
  static ScAddr concept_message;

  SC_PROPERTY(Keynode("nrel_answer_pattern"), ForceCreate)
  static ScAddr nrel_answer_pattern;

  SC_PROPERTY(Keynode("nrel_authors"), ForceCreate)
  static ScAddr nrel_authors;

  SC_PROPERTY(Keynode("nrel_message_key_elements"), ForceCreate)
  static ScAddr nrel_message_key_elements;

  SC_PROPERTY(Keynode("nrel_message_decomposition"), ForceCreate)
  static ScAddr nrel_message_decomposition;

  SC_PROPERTY(Keynode("nrel_message_sequence"), ForceCreate)
  static ScAddr nrel_message_sequence;

  SC_PROPERTY(Keynode("nrel_reply"), ForceCreate)
  static ScAddr nrel_reply;

  SC_PROPERTY(Keynode("rrel_message_theme"), ForceCreate)
  static ScAddr rrel_message_theme;

  SC_PROPERTY(Keynode("concept_text_file"), ForceCreate)
  static ScAddr concept_text_file;

  SC_PROPERTY(Keynode("template_reply_target"), ForceCreate)
  static ScAddr template_reply_target;

  SC_PROPERTY(Keynode("rrel_entity"), ForceCreate)
  static ScAddr rrel_entity;

  SC_PROPERTY(Keynode("nrel_semantic_equivalent"), ForceCreate)
  static ScAddr nrel_semantic_equivalent;

  SC_PROPERTY(Keynode("answer_structure"), ForceCreate)
  static ScAddr answer_structure;
};

}  // namespace dialogControlModule
