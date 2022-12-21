#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "ExampleKeynodes.generated.hpp"

namespace exampleModule
{
class ExampleKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_example"), ForceCreate)
  static ScAddr action_example;

    SC_PROPERTY(Keynode("concept_declarative_sentence"), ForceCreate)
    static ScAddr concept_declarative_sentence;

    SC_PROPERTY(Keynode("concept_interrogative_sentence"), ForceCreate)
    static ScAddr concept_interrogative_sentence;

    SC_PROPERTY(Keynode("concept_exclamatory_sentence"), ForceCreate)
    static ScAddr concept_exclamatory_sentence;
};

}  // namespace exampleModule
