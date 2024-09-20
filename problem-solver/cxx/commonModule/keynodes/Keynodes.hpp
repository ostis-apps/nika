#pragma once

#include <sc-memory/sc_keynodes.hpp>

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

namespace commonModule
{
class Keynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_deactivated{"action_deactivated", ScType::NodeConstClass};

  static inline ScKeynode const number{"number", ScType::NodeConstClass};

  static inline ScKeynode const nrel_sc_text_translation{"nrel_sc_text_translation", ScType::NodeConstNoRole};

  static inline ScKeynode const action_interpret_non_atomic_action{"action_interpret_non_atomic_action", ScType::NodeConstClass};

  static inline ScKeynode const languages{"languages", ScType::NodeConst};

  static inline ScKeynode const nrel_decomposition_of_action{"nrel_decomposition_of_action", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_then{"nrel_then", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_else{"nrel_else", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_goto{"nrel_goto", ScType::NodeConstNoRole};

  static inline ScKeynode const file{"file", ScType::NodeConst};

  static inline ScKeynode const power_of_set{"power_of_set", ScType::NodeConst};

  static inline ScKeynode const nrel_measurement{"nrel_measurement", ScType::NodeConstNoRole};

  static inline ScKeynode const lang_en{"lang_en", ScType::NodeConstClass};
};

}  // namespace commonModule
