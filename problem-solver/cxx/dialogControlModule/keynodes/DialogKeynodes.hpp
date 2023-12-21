#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "DialogKeynodes.generated.hpp"

namespace dialogControlModule
{
class DialogKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("nrel_sc_text_translation"), ForceCreate)
  static ScAddr nrel_sc_text_translation;

  SC_PROPERTY(Keynode("nrel_phrase_template"), ForceCreate)
  static ScAddr nrel_phrase_template;

  SC_PROPERTY(Keynode("rrel_key_sc_element"), ForceCreate)
  static ScAddr rrel_key_sc_element;
};

}  // namespace dialogControlModule
