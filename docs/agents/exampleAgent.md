Is an agent that adds a message to the class, according to its intonation

**Action class:**

`action_example`


**Parameters:**

1. `message`

**Workflow:**

* The agent reads the content of the message, then parses the punctuation marks. In accordance with them, assigns the corresponding class to the message.

### Example

Example of an input structure:

<img src="../images/exampleAgentInput.png"></img>

Example of an output structure:

<img src="../images/exampleAgentOutput.png"></img>

### Result

Possible result codes:

* `sc_result_ok` - answer message generated;
* `sc_result_error`- internal error.
