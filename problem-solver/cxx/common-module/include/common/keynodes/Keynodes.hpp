#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace commonModule
{
class Keynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_deactivated{"action_deactivated", ScType::ConstNodeClass};

  static inline ScKeynode const number{"number", ScType::ConstNodeClass};

  static inline ScKeynode const nrel_sc_text_translation{"nrel_sc_text_translation", ScType::ConstNodeNonRole};

  static inline ScKeynode const action_interpret_non_atomic_action{
      "action_interpret_non_atomic_action",
      ScType::ConstNodeClass};

  static inline ScKeynode const nrel_message_decomposition{"nrel_message_decomposition"};

  static inline ScKeynode const nrel_message_sequence{"nrel_message_sequence"};

  static inline ScKeynode const nrel_authors{"nrel_authors"};

  static inline ScKeynode const rrel_message_theme{"rrel_message_theme"};

  static inline ScKeynode const languages{"languages", ScType::ConstNode};

  static inline ScKeynode const nrel_decomposition_of_action{"nrel_decomposition_of_action", ScType::ConstNodeNonRole};

  static inline ScKeynode const nrel_then{"nrel_then", ScType::ConstNodeNonRole};

  static inline ScKeynode const nrel_else{"nrel_else", ScType::ConstNodeNonRole};

  static inline ScKeynode const nrel_goto{"nrel_goto", ScType::ConstNodeNonRole};

  static inline ScKeynode const file{"file", ScType::ConstNode};

  static inline ScKeynode const power_of_set{"power_of_set", ScType::ConstNode};

  static inline ScKeynode const nrel_measurement{"nrel_measurement", ScType::ConstNodeNonRole};

  static inline ScKeynode const lang_en{"lang_en", ScType::ConstNodeClass};
};

}  // namespace commonModule
