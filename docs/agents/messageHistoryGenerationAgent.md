Is an agent that adds message to the dialog,.

**Action class:**

`action_add_message_to_the_dialog`


**Parameters:**

1. `message node` - an element of `concept_message` and `concept_atomic_message` or `concept_non_atomic_message`;
2. `dialog history` - an element of `concept_dialogue` where the dialog history will be saved.

**Workflow:**

* The message node received by the agent is added to the message sequence in the dialog history. 
* If the dialog history is empty, then a relation `rrel_1` will be added to the first added message. The last message always has a relation `rrel_last` . There is a relation `nrel_message_sequence` between messages that defines the sequence of messages in the dialog. 

### Example

Example of an input structures to add a user message to the dialog:

<img src="../images/messageHistoryGenerationAgentUser.png"></img>
<img src="../images/messageHistoryGenerationAgentTestDialogUser.png"></img>

Example of an input structures to add a systems message to the dialog:

<img src="../images/messageHistoryGenerationAgentNika.png"></img>
<img src="../images/messageHistoryGenerationAgentTestDialogNika.png"></img>

Example of an output structure:

<img src="../images/messageHistoryGenerationAgentDialog.png"></img>

### Result

Possible result codes:
 
* `SC_RESULT_OK` - the reply message is generated.
* `SC_RESULT_ERROR_INVALID_PARAMS` - the action has no incoming message.
