#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace messageClassificationModule
{
class MessageClassificationKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_message_topic_classification{"action_message_topic_classification"};

  static inline ScKeynode const action_alternative_message_topic_classification{
      "action_alternative_message_topic_classification"};

  static inline ScKeynode const action_rasa_message_topic_classification{
    "action_rasa_message_topic_classification"};

  static inline ScKeynode const concept_intent_possible_class{"concept_intent_possible_class"};

  static inline ScKeynode const concept_trait_possible_class{"concept_trait_possible_class"};

  static inline ScKeynode const concept_entity_possible_class{"concept_entity_possible_class"};

  static inline ScKeynode const nrel_wit_ai_idtf{"nrel_wit_ai_idtf"};

  static inline ScKeynode const nrel_rasa_idtf{"nrel_rasa_idtf"};

  static inline ScKeynode const nrel_entity_possible_role{"nrel_entity_possible_role"};

  static inline ScKeynode const concept_not_classified_by_intent_message{"concept_not_classified_by_intent_message"};

  static inline ScKeynode const concept_not_classified_by_trait_message{"concept_not_classified_by_trait_message"};

  static inline ScKeynode const concept_classify_message_rule{"concept_classify_message_rule"};
};

}  // namespace messageClassificationModule
