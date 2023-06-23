#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"
#include "LetterKeynodes.generated.hpp"

namespace exampleModule
{
    class LetterKeynodes : public ScObject
    {
        SC_CLASS()
        SC_GENERATED_BODY()
    public:
        SC_PROPERTY(Keynode("action_letter_search"), ForceCreate)
        static ScAddr action_letter_search;
        
        SC_PROPERTY(Keynode("concept_message_about_letter_search"), ForceCreate)
        static ScAddr concept_message_about_letter_search;

        SC_PROPERTY(Keynode("rrel_entity"), ForceCreate)
        static ScAddr rrel_entity;
    };
}