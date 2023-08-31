#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "Keynodes.generated.hpp"

namespace commonModule
{
class Keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_deactivated"), ForceCreate)
  static ScAddr action_deactivated;

  SC_PROPERTY(Keynode("action_interpret_non_atomic_action"), ForceCreate)
  static ScAddr action_interpret_non_atomic_action;

  SC_PROPERTY(Keynode("languages"), ForceCreate)
  static ScAddr languages;

  SC_PROPERTY(Keynode("nrel_decomposition_of_action"), ForceCreate)
  static ScAddr nrel_decomposition_of_action;

  SC_PROPERTY(Keynode("nrel_then"), ForceCreate)
  static ScAddr nrel_then;

  SC_PROPERTY(Keynode("nrel_else"), ForceCreate)
  static ScAddr nrel_else;

  SC_PROPERTY(Keynode("nrel_goto"), ForceCreate)
  static ScAddr nrel_goto;

  SC_PROPERTY(Keynode("file"), ForceCreate)
  static ScAddr file;

  SC_PROPERTY(Keynode("power_of_set"), ForceCreate)
  static ScAddr power_of_set;

  SC_PROPERTY(Keynode("nrel_measurement"), ForceCreate)
  static ScAddr nrel_measurement;

  SC_PROPERTY(Keynode("lang_en"), ForceCreate)
  static ScAddr lang_en;
};

}  // namespace commonModule
