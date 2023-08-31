# Phrase generation agent

It is an agent that generates a sc-link with the reply message text, where sc-variables names substitued with constant sc-links contents from knowledge base.
This agent is called by StandartMessageReplyAgent to do this work and it returns the own result to him for answering the user question.

**Action class:**

`action_phrase_generation`

**Parameters:**

1. `replyMessageNode` that is the result of StandartMessageReplyAgent;

2. `phraseLink` is a sc-link with a text template.

**Workflow:**

* The agent tryes to get `templateNode`, that connected with `phraseLink` by `nrel_phrase_template` norole relation;

<img src="../images/phraseGenerationAgentStep1.png"></img>

* Then it seeks for arguments of the template - `parametersNode`, this node is connected with `actionAddr` by `rrel_3` role relation;
* After this the PhraseGenerationAgent generates sc-link by template using `templateNode`, `parametersNode` and `phraseLink`;
* The language of created sc-link depends on language of `phraseLink`. If there is a `lang_ru` node and a constant positive role relation from
`lang_ru` to `phraseLink`, then the agent creates similiar role relation from `lang_ru` to `linkResult`. If the agent can't find the language of this sc-link, he doesn't create any relation;
* In the sc-link the agent finds all sc-variables and using the `templateNode` choose necessary sc-elements from knowledge base. SC-variables are replaced by identifiers of these sc-elements, that were found;
* The final sc-link connects with `actionAddr` by `nrel_result` norole relation, PhraseGenerationAgent finishes his work.

### Example

Example of an input structure:

<img src="../images/phraseGenerationAgentStep2.png"></img>

Example of an output structure:

<img src="../images/phraseGenerationAgentStep3.png"></img>

### Result

Possible result codes:

* `SC_RESULT_OK` - the reply message is generated.
* `SC_RESULT_ERROR` - internal error, answer can't be found.
* `SC_RESULT_ERROR_INVALID_PARAMS` - the action has no reply message or phrase link.
