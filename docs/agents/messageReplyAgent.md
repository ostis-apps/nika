Is an agent that generates the response message, associates with the original relation "nrel_reply"

**Action class:**

`action_reply_to_message`


**Parameters:**

1. `linkAddr` -- sc-link with user message text;
2. `dialogAddr` -- dialog node, an element of `concept_dialogue`.

Also there is the `author` of the message.

**Workflow:**

* The agent generates a message node in the knowledge base, identifying the author of the action as the author of the message and the received text file as the text of that message;
* Then the necessary construction is generated to call the agent of non-atomic action interpretation. An example of this construction is shown below.

<img src="../images/messageReplyAgentGener.png"></img>

* The agent waits until the interpretation agent finishes its work. Then searches for the response message that should have been generated during the interpretation agent's work and adds it to the response.

### Example

Example of an input structure:

<img src="../images/messageReplyAgentInput.png"></img>

Example of an output structure:

<img src="../images/messageReplyAgentOutput.png"></img>

### Result

Possible result codes:
 
* `SC_RESULT_OK` - answer message generated;
* `SC_RESULT_ERROR`- internal error.
