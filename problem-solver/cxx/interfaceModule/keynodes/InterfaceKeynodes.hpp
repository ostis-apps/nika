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
  SC_PROPERTY(Keynode("concept_header"), ForceCreate)
  static ScAddr concept_header;
  SC_PROPERTY(Keynode("concept_main_part"), ForceCreate)
  static ScAddr concept_main_part;
  SC_PROPERTY(Keynode("concept_footer"), ForceCreate)
  static ScAddr concept_footer;
  SC_PROPERTY(Keynode("nrel_component_color"), ForceCreate)
  static ScAddr nrel_component_color;
  SC_PROPERTY(Keynode("rrel_entity"), ForceCreate)
  static ScAddr rrel_entity;
};

}  // namespace interfaceModule
