#pragma once

#include <sc-memory/sc_keynodes.hpp>

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

namespace interfaceModule
{
class InterfaceKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_change_interface{"action_change_interface"};

  static inline ScKeynode const concept_header{"concept_header"};

  static inline ScKeynode const concept_main_part{"concept_main_part"};

  static inline ScKeynode const concept_footer{"concept_footer"};

  static inline ScKeynode const nrel_component_color{"nrel_component_color"};

  static inline ScKeynode const rrel_entity{"rrel_entity"};

  static inline ScKeynode const rrel_color{"rrel_color"};
};

}  // namespace interfaceModule
