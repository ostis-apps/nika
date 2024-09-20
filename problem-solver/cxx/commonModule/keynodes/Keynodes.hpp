#pragma once

#include <sc-memory/sc_keynodes.hpp>

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

namespace commonModule
{
class Keynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_deactivated{"action_deactivated"};

  static inline ScKeynode const number{"number"};

  static inline ScKeynode const nrel_sc_text_translation{"nrel_sc_text_translation"};

  static inline ScKeynode const action_interpret_non_atomic_action{"action_interpret_non_atomic_action"};

  static inline ScKeynode const languages{"languages"};

  static inline ScKeynode const nrel_decomposition_of_action{"nrel_decomposition_of_action"};

  static inline ScKeynode const nrel_then{"nrel_then"};

  static inline ScKeynode const nrel_else{"nrel_else"};

  static inline ScKeynode const nrel_goto{"nrel_goto"};

  static inline ScKeynode const file{"file"};

  static inline ScKeynode const power_of_set{"power_of_set"};

  static inline ScKeynode const nrel_measurement{"nrel_measurement"};

  static inline ScKeynode const lang_en{"lang_en"};
};

}  // namespace commonModule
