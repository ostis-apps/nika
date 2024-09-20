#pragma once

#include <sc-memory/sc_keynodes.hpp>

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

namespace dialogControlModule
{
class DialogKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const nrel_sc_text_translation{"nrel_sc_text_translation", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_phrase_template{"nrel_phrase_template", ScType::NodeConstNoRole};
};

}  // namespace dialogControlModule
