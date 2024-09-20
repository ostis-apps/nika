#pragma once

#include <sc-memory/sc_keynodes.hpp>

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

namespace dialogControlModule
{
class MessageKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_phrase_generation{"action_phrase_generation"};

  static inline ScKeynode const action_standard_message_reply{"action_standard_message_reply"};

  static inline ScKeynode const concept_answer_on_standard_message_rule{"concept_answer_on_standard_message_rule"};

  static inline ScKeynode const concept_answer_on_standard_message_rule_class_by_priority{
      "concept_answer_on_standard_message_rule_class_by_priority"};

  static inline ScKeynode const concept_atomic_message{"concept_atomic_message"};

  static inline ScKeynode const concept_non_atomic_message{"concept_non_atomic_message"};

  static inline ScKeynode const concept_message{"concept_message"};

  static inline ScKeynode const nrel_answer_pattern{"nrel_answer_pattern"};

  static inline ScKeynode const nrel_authors{"nrel_authors"};

  static inline ScKeynode const nrel_message_key_elements{"nrel_message_key_elements"};

  static inline ScKeynode const nrel_message_decomposition{"nrel_message_decomposition"};

  static inline ScKeynode const nrel_message_sequence{"nrel_message_sequence"};

  static inline ScKeynode const nrel_reply{"nrel_reply"};

  static inline ScKeynode const rrel_message_theme{"rrel_message_theme"};

  static inline ScKeynode const concept_text_file{"concept_text_file"};

  static inline ScKeynode const template_reply_target{"template_reply_target"};

  static inline ScKeynode const rrel_entity{"rrel_entity"};

  static inline ScKeynode const nrel_semantic_equivalent{"nrel_semantic_equivalent"};

  static inline ScKeynode const answer_structure{"answer_structure"};
};

}  // namespace dialogControlModule
