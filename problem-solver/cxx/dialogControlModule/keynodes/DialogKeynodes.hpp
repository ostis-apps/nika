#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace dialogControlModule
{
class DialogKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const nrel_sc_text_translation{"nrel_sc_text_translation", ScType::ConstNodeNonRole};

  static inline ScKeynode const nrel_phrase_template{"nrel_phrase_template", ScType::ConstNodeNonRole};
};

}  // namespace dialogControlModule
