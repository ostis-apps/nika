# Message classification agent

This is an agent that performs the message classification using [logic rules](../subsystems/scl-machine.md).

**Action class:**

`action_alternative_message_topic_classification`

**Parameters:**

1. `messageAddr` -- an element of `concept_message`.

### Examples

Examples of an input structure:

<img src="../images/alternativeMessageTopicClassificationAgentInput.png"></img>

Example of a logic formula to classify message:

<img src="../images/lr_greeting_message.png"></img>

The output of the agent depends on the inference agent results. If any formula has been applied successfully, `messageAddr` will have a class according to the formula.
Otherwise, `messageAddr` has `concept_not_classified_by_intent_message` class.

### Agent implementation language
C++

### Result

Possible result codes:

* `SC_RESULT_OK` - the message is successfully classified (or there is empty classification) or the action doesn't belong to the action_alternative_message_topic_classification.
* `SC_RESULT_ERROR`- internal error.
