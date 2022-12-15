#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "Keynodes.generated.hpp"

namespace satelliteCoordinatesAgentsModule
{

class Keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_get_satellite_from_message"), ForceCreate)
  static ScAddr action_get_satellite_from_message;

  SC_PROPERTY(Keynode("action_add_satellite_coordinates"), ForceCreate)
  static ScAddr action_add_satellite_coordinates;

  SC_PROPERTY(Keynode("action_remove_coordinates_from_satellite"), ForceCreate)
  static ScAddr action_remove_coordinates_from_satellite;

  SC_PROPERTY(Keynode("nrel_id"), ForceCreate)
  static ScAddr nrel_id;

  SC_PROPERTY(Keynode("nrel_coordinates"), ForceCreate)
  static ScAddr nrel_coordinates;

  SC_PROPERTY(Keynode("rrel_entity"), ForceCreate)
  static ScAddr rrel_entity;

  SC_PROPERTY(Keynode("concept_message_about_coordinates_of_satellite"), ForceCreate)
  static ScAddr concept_message_about_coordinates_of_satellite;
};

}