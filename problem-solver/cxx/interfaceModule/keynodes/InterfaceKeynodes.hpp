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

  SC_PROPERTY(Keynode("action_create_answer_template"), ForceCreate)
  static ScAddr action_create_answer_template;

  SC_PROPERTY(Keynode("concept_header"), ForceCreate)
  static ScAddr concept_header;

  SC_PROPERTY(Keynode("concept_main_part"), ForceCreate)
  static ScAddr concept_main_part;

  SC_PROPERTY(Keynode("concept_footer"), ForceCreate)
  static ScAddr concept_footer;

  SC_PROPERTY(Keynode("concept_phrase"), ForceCreate)
  static ScAddr concept_phrase;

  SC_PROPERTY(Keynode("nrel_component_color"), ForceCreate)
  static ScAddr nrel_component_color;

  SC_PROPERTY(Keynode("nrel_inclusion"), ForceCreate)
  static ScAddr nrel_inclusion;

  SC_PROPERTY(Keynode("nrel_main_idtf"), ForceCreate)
  static ScAddr nrel_main_idtf;

  SC_PROPERTY(Keynode("nrel_answer"), ForceCreate)
  static ScAddr nrel_answer;

  SC_PROPERTY(Keynode("rrel_entity"), ForceCreate)
  static ScAddr rrel_entity;

  SC_PROPERTY(Keynode("rrel_color"), ForceCreate)
  static ScAddr rrel_color;

  SC_PROPERTY(Keynode("lang_ru"), ForceCreate)
  static ScAddr lang_ru;
  
  SC_PROPERTY(Keynode("lang_en"), ForceCreate)
  static ScAddr lang_en;
};

}  // namespace interfaceModule
