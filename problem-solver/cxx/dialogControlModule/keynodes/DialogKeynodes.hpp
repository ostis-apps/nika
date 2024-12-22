#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace dialogControlModule
{
class DialogKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const nrel_sc_text_translation{"nrel_sc_text_translation", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_phrase_template{"nrel_phrase_template", ScType::NodeConstNoRole};

  static inline ScKeynode const not_to_search_structure{"not_to_search_structure", ScType::NodeStruct};

  static inline ScKeynode const nrel_set_power{"nrel_set_power", ScType::NodeConstNoRole};

  static inline ScKeynode const rrel_maximum_studied_object_class{"rrel_maximum_studied_object_class", ScType::NodeConstRole};

  static inline ScKeynode const rrel_not_maximum_studied_object_class{"rrel_not_maximum_studied_object_class", ScType::NodeConstRole};
};

}  // namespace dialogControlModule
