#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "Keynodes.generated.hpp"

namespace mathModule
{

class Keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("nrel_math_constant_name"), ForceCreate)
  static ScAddr nrel_math_constant_name;

  SC_PROPERTY(Keynode("nrel_math_constant_value"), ForceCreate)
  static ScAddr nrel_math_constant_value;

  SC_PROPERTY(Keynode("rrel_entity"), ForceCreate)
  static ScAddr rrel_entity;

  SC_PROPERTY(Keynode("action_get_math_constant_value"), ForceCreate)
  static ScAddr action_get_math_constant_value;

  SC_PROPERTY(Keynode("concept_message_about_math_constant"), ForceCreate)
  static ScAddr concept_message_about_math_constant;
};

}