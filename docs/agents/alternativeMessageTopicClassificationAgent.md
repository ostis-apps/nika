This is an agent that performs the message classification using [logic rules](../subsystems/scl-machine.md).

**Action class:**

`action_alternative_message_topic_classification`

**Parameters:**

1. `message node` - an element of `concept_message`;

### Examples

Examples of an input structure:

<img src="../images/alternativeMessageTopicClassificationAgentInput.png"></img>

Example of a logic rule to classify message:

<img src="../images/lr_greeting_message.png"></img>

### Agent implementation language
C++

### Result

Possible result codes:

* `SC_RESULT_OK` - the message is successfully classified (or there is empty classification) or the action doesn't belong to the action_alternative_message_topic_classification.
* `SC_RESULT_ERROR`- internal error.