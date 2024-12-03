# Message topic classification agent

This is an agent that performs the message classification by topic (greeting, about lab work condition, about entity etc.).
Also, the agent classifies message by trait (neutral, positive, negative emotional coloring) and gets message entities.
The agent classifies a message using Wit.ai.

**Action class:**

`action_message_topic_classification`

**Parameters:**

1. `messageAddr` -- an element of `concept_message`.

**Libraries used:**

* [Wit.ai](https://wit.ai/) - to classify message and get entities.

**Wit.ai features**

Unlike other available message classifier(Rasa) With.ai has the ability to determine the message trait. However, a significant disadvantage of using it is its dependence on an Internet connection. Also Wit.ai it has a pre-created interface for training an existing classifier.

The json format of the response from the classifier:

```json
{"entities":{"rrel_entity:rrel_entity":[{"body":"Ника","confidence":1,"end":23,"entities":{},"id":"545916503952309","name":"rrel_entity","role":"rrel_entity","start":19,"type":"value","value":"Ника"}]},"intents":[{"confidence":1,"id":"271954618797345","name":"about_skill"}],"text":"На что разбивается Ника?","traits":{}}
```

**Comment:**

* The input message must contain a text file with the text in Russian;
* The excluding entity should be formalized in knowledge base.

### Examples

Examples of an input structure:

<img src="../images/messageTopicClassificationAgentInput.png"></img>

Examples of an output structure:

<img src="../images/messageTopicClassificationAgentOutput.png"></img>

Example of a structure needed to classify message by intent:

<img src="../images/messageTopicClassificationAgentIntentFormalization.png"></img>

Example of a structure needed to classify message by trait:

<img src="../images/messageTopicClassificationAgentTraitFormalization.png"></img>

Example of a structure needed to get message entity:

<img src="../images/messageTopicClassificationAgentEntityFormalization.png"></img>

**Message classes:**

1. `concept_greeting_message`
2. `concept_message_about_entity`
3. `concept_message_about_subdividing`
4. `concept_message_about_study_system`
5. `concept_message_about_lab_work_condition`
6. `concept_message_about_lab_work_deadline`

### Agent implementation language
C++

### Result

Possible result codes:

* `SC_RESULT_OK` - the message is successfully classified (or there is empty classification) or the action doesn't belong to the action_message_topic_classification.
* `SC_RESULT_ERROR`- internal error.
