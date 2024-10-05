#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace messageReplyModule
{
class MessageReplyKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_reply_to_message{"action_reply_to_message", ScType::NodeConstClass};

  static inline ScKeynode const message_processing_program{"message_processing_program", ScType::NodeConst};

  static inline ScKeynode const nrel_authors{"nrel_authors", ScType::NodeConstNoRole};

  static inline ScKeynode const concept_message{"concept_message", ScType::NodeConstClass};

  static inline ScKeynode const nrel_reply{"nrel_reply", ScType::NodeConstNoRole};

  static inline ScKeynode const format_wav{"format_wav", ScType::NodeConstClass};

  static inline ScKeynode const languages{"languages", ScType::NodeConst};

  static inline ScKeynode const concept_sound_file{"concept_sound_file", ScType::NodeConstClass};

  static inline ScKeynode const concept_text_file{"concept_text_file", ScType::NodeConstClass};

  static inline ScKeynode const concept_dialogue{"concept_dialogue", ScType::NodeConstClass};

  static inline ScKeynode const rrel_last{"rrel_last", ScType::NodeConstRole};

  static inline ScKeynode const nrel_message_sequence{"nrel_message_sequence", ScType::NodeConstNoRole};

  static inline ScKeynode const myself{"myself", ScType::NodeConst};
};

}  // namespace messageReplyModule
