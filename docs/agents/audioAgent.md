
# Text to Speech Agent

This agent voices response messages in Russian and English.

**Action class:**

`action_text_to_speech`

**Parameters:**

1. `messageAddr` -- class element `concept_message`.

**Libraries used:**

* [responsiveVoice](https://responsivevoice.org/) - для озвучивания сообщений.

**Workflow:**

* The agent searches the knowledge base for an sc-link with the message text through the nrel_sc_text_translation relationship;
* Next, it looks for the language class to which this sc-link belongs;
* The agent then voices the message text according to the language.

**Example of an input structure:**

<img src="../images/audioAgentInput.png"></img>

**Agent implementation language:**

TypeScript
