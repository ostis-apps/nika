#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace interfaceModule
{
class InterfaceKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_change_interface{"action_change_interface", ScType::ConstNodeClass};

  static inline ScKeynode const concept_header{"concept_header", ScType::ConstNodeClass};

  static inline ScKeynode const concept_main_part{"concept_main_part", ScType::ConstNodeClass};

  static inline ScKeynode const concept_footer{"concept_footer", ScType::ConstNodeClass};

  static inline ScKeynode const nrel_component_color{"nrel_component_color", ScType::ConstNodeNonRole};

  static inline ScKeynode const rrel_entity{"rrel_entity", ScType::ConstNodeRole};

  static inline ScKeynode const rrel_color{"rrel_color", ScType::ConstNodeRole};
};

}  // namespace interfaceModule
