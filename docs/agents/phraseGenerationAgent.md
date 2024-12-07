# Phrase generation agent

It is an agent that generates a sc-link with the reply message text, where sc-variables names substituted with constant sc-links contents from knowledge base.
This agent is called by StandardMessageReplyAgent to do this work and it returns the own result to him for answering the user question.

**Action class:**

`action_phrase_generation`

**Parameters:**

1. `replyMessageNode` that is the result of StandardMessageReplyAgent;

2. `phraseLink` is a sc-link with a text template.

3. `parametersNode` is sc-node containing template arguments.


**Workflow:**

* The agent tries to get `templateNode`, that connected with `phraseLink` by `nrel_phrase_template` norole relation;

<img src="../images/phraseGenerationAgentStep1.png"></img>

* Then it seeks for arguments of the template - `parametersNode`, this node is connected with `actionAddr` by `rrel_3` role relation;
* After this the PhraseGenerationAgent generates sc-link by template using `templateNode`, `parametersNode` and `phraseLink`;
    * Processes the regular expressions in `phraseLink` to identify variable names.  
    * Matches the type of regular expression to the variable name.  
    * Processes the `templateNode` using the `parametersNode` to map variables in the `templateNode` to the values of their corresponding constants.  
    * Creates the response text by invoking a handler for each variable whose constant value was found. Each handler corresponds to its specific type of regular expression.
* The language of created sc-link depends on language of `phraseLink`. If there is a `lang_ru` node and a constant positive role relation from
`lang_ru` to `phraseLink`, then the agent creates similar role relation from `lang_ru` to `linkResult`. If the agent can't find the language of this sc-link, he doesn't create any relation;
* Upon successfully generating the response, the agent creates a response structure in the knowledge base corresponding to the identified values. 
* A knowledge base structure is created that includes all the nodes involved in creating the response. This structure is needed to be displayed in the interface after the response.   
* The final sc-link connects with `actionAddr` by `nrel_result` norole relation, PhraseGenerationAgent finishes his work.

### Example

Example of an input structure:

<img src="../images/phraseGenerationAgentStep2.png"></img>

Example of an output structure:

<img src="../images/phraseGenerationAgentStep3.png"></img>

### Structure of Non-Processable Elements  

A specific field of the agent is `notSearchable` - `not_to_search_structure` in the knowledge base. `notSearchable` is used when searching for variable values in the `templateNode` to exclude certain elements that meet the description but should not be processed.  

### Result

Possible result codes:

* `SC_RESULT_OK` - the reply message is generated.
* `SC_RESULT_ERROR` - internal error, answer can't be found.
* `SC_RESULT_ERROR_INVALID_PARAMS` - the action has no reply message or phrase link.
