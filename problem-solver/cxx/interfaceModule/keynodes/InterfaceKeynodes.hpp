#pragma once

#include <sc-memory/sc_keynodes.hpp>

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

namespace interfaceModule
{
class InterfaceKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_change_interface{"action_change_interface", ScType::NodeConstClass};

  static inline ScKeynode const concept_header{"concept_header", ScType::NodeConstClass};

  static inline ScKeynode const concept_main_part{"concept_main_part", ScType::NodeConstClass};

  static inline ScKeynode const concept_footer{"concept_footer", ScType::NodeConstClass};

  static inline ScKeynode const nrel_component_color{"nrel_component_color", ScType::NodeConstNoRole};

  static inline ScKeynode const rrel_entity{"rrel_entity", ScType::NodeConstRole};

  static inline ScKeynode const rrel_color{"rrel_color", ScType::NodeConstRole};
};

}  // namespace interfaceModule
