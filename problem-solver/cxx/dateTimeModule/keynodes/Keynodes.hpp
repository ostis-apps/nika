#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "Keynodes.generated.hpp"

namespace dateTimeAgentsModule
{

class Keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_get_timezone_from_message"), ForceCreate)
  static ScAddr action_get_timezone_from_message;

  SC_PROPERTY(Keynode("action_add_timezone_date_time"), ForceCreate)
  static ScAddr action_add_timezone_date_time;

  SC_PROPERTY(Keynode("action_remove_timezone_date_time"), ForceCreate)
  static ScAddr action_remove_timezone_date_time;

  SC_PROPERTY(Keynode("nrel_date"), ForceCreate)
  static ScAddr nrel_date;

  SC_PROPERTY(Keynode("nrel_time"), ForceCreate)
  static ScAddr nrel_time;

  SC_PROPERTY(Keynode("nrel_timezone_name"), ForceCreate)
  static ScAddr nrel_timezone_name;

  SC_PROPERTY(Keynode("rrel_entity"), ForceCreate)
  static ScAddr rrel_entity;

  SC_PROPERTY(Keynode("concept_message_about_time_in_timezone"), ForceCreate)
  static ScAddr concept_message_about_time_in_timezone;
};

}