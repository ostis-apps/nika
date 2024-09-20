#pragma once

#include <sc-memory/sc_keynodes.hpp>

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

namespace messageReplyModule
{
class MessageReplyKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_reply_to_message{"action_reply_to_message"};

  static inline ScKeynode const message_processing_program{"message_processing_program"};

  static inline ScKeynode const nrel_authors{"nrel_authors"};

  static inline ScKeynode const concept_message{"concept_message"};

  static inline ScKeynode const nrel_reply{"nrel_reply"};

  static inline ScKeynode const format_wav{"format_wav"};

  static inline ScKeynode const languages{"languages"};

  static inline ScKeynode const concept_sound_file{"concept_sound_file"};

  static inline ScKeynode const concept_text_file{"concept_text_file"};

  static inline ScKeynode const concept_dialogue{"concept_dialogue"};

  static inline ScKeynode const rrel_last{"rrel_last"};

  static inline ScKeynode const nrel_message_sequence{"nrel_message_sequence"};

  static inline ScKeynode const myself{"myself", ScType::NodeConst};
};

}  // namespace messageReplyModule
