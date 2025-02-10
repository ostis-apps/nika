#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace messageReplyModule
{
class MessageReplyKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_interpret_non_atomic_action{
      "action_interpret_non_atomic_action",
      ScType::ConstNodeClass};

  static inline ScKeynode const action_reply_to_message{"action_reply_to_message", ScType::ConstNodeClass};

  static inline ScKeynode const message_processing_program{"message_processing_program", ScType::ConstNode};

  static inline ScKeynode const nrel_authors{"nrel_authors", ScType::ConstNodeNonRole};

  static inline ScKeynode const concept_message{"concept_message", ScType::ConstNodeClass};

  static inline ScKeynode const nrel_reply{"nrel_reply", ScType::ConstNodeNonRole};

  static inline ScKeynode const format_wav{"format_wav", ScType::ConstNodeClass};

  static inline ScKeynode const languages{"languages", ScType::ConstNode};

  static inline ScKeynode const nrel_sc_text_translation{"nrel_sc_text_translation", ScType::ConstNodeNonRole};

  static inline ScKeynode const concept_sound_file{"concept_sound_file", ScType::ConstNodeClass};

  static inline ScKeynode const concept_text_file{"concept_text_file", ScType::ConstNodeClass};

  static inline ScKeynode const concept_dialogue{"concept_dialogue", ScType::ConstNodeClass};

  static inline ScKeynode const rrel_last{"rrel_last", ScType::ConstNodeRole};

  static inline ScKeynode const nrel_message_sequence{"nrel_message_sequence", ScType::ConstNodeNonRole};

  static inline ScKeynode const myself{"myself", ScType::ConstNode};
};

}  // namespace messageReplyModule
