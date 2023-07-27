#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"
#include "MessageProcessingKeynodes.generated.hpp"

namespace messageProcessingModule
{
class MessageProcessingKeynodes : public ScObject
{
    SC_CLASS()
    SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_find_word_in_set_by_first_letter"), ForceCreate)
  static ScAddr action_find_word_in_set_by_first_letter;
  
  SC_PROPERTY(Keynode("concept_message_about_find_word_by_first_letter"), ForceCreate)
  static ScAddr concept_message_about_find_word_by_first_letter;

  SC_PROPERTY(Keynode("word_starts_with_required_letter_answer_phrase"), ForceCreate)
  static ScAddr word_starts_with_required_letter_answer_phrase;
};

} // namespace messageProcessingModule
