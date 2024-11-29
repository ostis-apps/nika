#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace dialogControlModule
{

class DialogKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const nrel_sc_text_translation{"nrel_sc_text_translation", ScType::ConstNodeNonRole};

  static inline ScKeynode const nrel_phrase_template{"nrel_phrase_template", ScType::ConstNodeNonRole};

  static inline ScKeynode const not_to_search_structure{"not_to_search_structure", ScType::ConstNodeStructure};

  static inline ScKeynode const nrel_set_power{"nrel_set_power", ScType::ConstNodeNonRole};

  static inline ScKeynode const rrel_maximum_studied_object_class{"rrel_maximum_studied_object_class", ScType::ConstNodeRole};

  static inline ScKeynode const rrel_not_maximum_studied_object_class{"rrel_not_maximum_studied_object_class", ScType::ConstNodeRole};
};

}  // namespace dialogControlModule
