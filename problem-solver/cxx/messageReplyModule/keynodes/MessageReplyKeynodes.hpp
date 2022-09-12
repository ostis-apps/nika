#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "MessageReplyKeynodes.generated.hpp"

namespace messageReplyModule
{
class MessageReplyKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_reply_to_message"), ForceCreate)
  static ScAddr action_reply_to_message;

  SC_PROPERTY(Keynode("message_processing_program"), ForceCreate)
  static ScAddr message_processing_program;

  SC_PROPERTY(Keynode("nrel_authors"), ForceCreate)
  static ScAddr nrel_authors;

  SC_PROPERTY(Keynode("concept_message"), ForceCreate)
  static ScAddr concept_message;

  SC_PROPERTY(Keynode("nrel_reply"), ForceCreate)
  static ScAddr nrel_reply;

  SC_PROPERTY(Keynode("format_wav"), ForceCreate)
  static ScAddr format_wav;

  SC_PROPERTY(Keynode("languages"), ForceCreate)
  static ScAddr languages;

  SC_PROPERTY(Keynode("concept_sound_file"), ForceCreate)
  static ScAddr concept_sound_file;

  SC_PROPERTY(Keynode("concept_text_file"), ForceCreate)
  static ScAddr concept_text_file;

  SC_PROPERTY(Keynode("concept_dialogue"), ForceCreate)
  static ScAddr concept_dialogue;

  SC_PROPERTY(Keynode("rrel_last"), ForceCreate)
  static ScAddr rrel_last;

  SC_PROPERTY(Keynode("nrel_message_sequence"), ForceCreate)
  static ScAddr nrel_message_sequence;

  SC_PROPERTY(Keynode("myself"), ForceCreate(ScType::NodeConst))
  static ScAddr myself;
};

}  // namespace messageReplyModule
