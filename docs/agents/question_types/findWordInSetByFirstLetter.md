# Find word in set by first letter agent

This is an agent added as an example to show how to add new agents to the system.
The agent can be used as a template to write other agents, for example to cities game where we need to search words by last letter of previous word or for other suitable purposes.

To test you can use message like `Что в примере начинается на букву А?` or use any other letter.

**Action class:**

`action_find_word_in_set_by_first_letter`

**Parameters:**

1. `messageAddr` -- an element of `concept_message` and `concept_message_about_find_word_by_first_letter`.

**Workflow:**

* The agent extracts an entity and a letter from the message finds all entities associated with it and starts with the found letter.
* After finding associated entities agent generate phrase that consist of identifiers of entities.
* If there are no words that start with the found letter, the agent displays a message about their absence.

The letter agent rule:
<img src="../images/lr_find_word_by_letter_message.png"></img>

The letter agent template phrase:
<img src="../images/concept_phrase_about_find_word_by_letter.png"></img>

### Example

Example of an input structure:

<img src="../images/FindWordInSetByLetterExample.png"></img>

### Result

Possible result codes:

* `SC_RESULT_OK` - agent successfully found elements and generate phrase;
* `SC_RESULT_ERROR`- `message` or `entity` is not found.
