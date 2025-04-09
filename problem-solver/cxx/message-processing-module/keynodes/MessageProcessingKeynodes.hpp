#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace messageProcessingModule
{
class MessageProcessingKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_find_word_in_set_by_first_letter{
      "action_find_word_in_set_by_first_letter",
      ScType::ConstNodeClass};

  static inline ScKeynode const concept_message_about_find_word_by_first_letter{
      "concept_message_about_find_word_by_first_letter",
      ScType::ConstNodeClass};

  static inline ScKeynode const word_starts_with_required_letter_answer_phrase{
      "word_starts_with_required_letter_answer_phrase",
      ScType::ConstNodeClass};

  static inline ScKeynode const rrel_entity{"rrel_entity"};
};

}  // namespace messageProcessingModule
