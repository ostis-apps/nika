# Message topic classification agent

This is an agent that performs the message classification by topic (greeting, about lab work condition, about entity etc.).
The agent classifies a message using Rasa.

**Action class:**

`action_rasa_message_topic_classification`

**Parameters:**

1. `messageAddr` -- an element of `concept_message`.

**Libraries used:**

* [Rasa](https://rasa.com/) - Rasa official website with documentation.

**Rasa features**

The use of the Rasa classifier is absolutely independent of the Internet connection, as well as its training and testing. Rasa does not have a pre-designed interface for training the classifier, it can be trained via .yml files. The Rasa module contains many other modules on which it depends, so it is necessary to allocate space in advance for local storage of the classifier.  

The json format of the response from the classifier:

```json
{"text":"Что такое Ника?","intent":{"name":"concept_message_about_entity","confidence":1.0},"entities":[{"entity":"entity","start":10,"end":14,"confidence_entity":0.9997850060462952,"value":"Ника","extractor":"DIETClassifier"}],"text_tokens":[[0,3],[4,9],[10,14]],"intent_ranking":[{"name":"ask_definition","confidence":1.0},{"name":"goodbye","confidence":2.69290423293711e-10},{"name":"greet","confidence":1.639179937618085e-11}]}
```

**Comment:**

* The input message must contain a text file with the text in Russian;
* The excluding entity should be formalized in knowledge base.

### Examples

Examples of an input structure:

<img src="../images/rasaMessageTopicClassificationAgentInput.png"></img>

Examples of an output structure:

<img src="../images/rasaMessageTopicClassificationAgentOutput.png"></img>

Example of a structure needed to classify message by intent:

<img src="../images/rasaMessageTopicClassificationAgentIntentFormalization.png"></img>

Example of a structure needed to get message entity:

<img src="../images/rasaMessageTopicClassificationAgentEntityFormalization.png"></img>

**Message classes:**

1. `concept_message_about_entity`

### Agent implementation language
C++

### Result

Possible result codes:

* `SC_RESULT_OK` - the message is successfully classified (or there is empty classification) or the action doesn't belong to the action_message_topic_classification.
* `SC_RESULT_ERROR`- internal error.
