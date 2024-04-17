# Create class instance agent

This agent creates an instance of the entity class.

**Action class:**

`action_create_class_instance`

**Parameters:**

1. `messageAddr` -- an element of `concept_message`.
2. `formLinkAddr` -- link with information from popup component.

### Examples

**Example of an input structure:**

<img src="../images/createClassInstanceAgentInput.png"></img>

**Examples of an output structure:**

Output about the successful creation of structures.

<img src="../images/createClassInstanceAgentOutput1.png"></img>

Error output about invalid or existing parameters.

<img src="../images/createClassInstanceAgentOutput2.png"></img>

Output to notify when the user has completed an action.

<img src="../images/createClassInstanceAgentOutput3.png"></img>

The result of the agent's work depends on the data received from the component.If the received data is correct, the structures will be created. Otherwise, a corresponding error message will be displayed.

### Agent implementation language
C++

### Result

Possible result codes:

* `SC_RESULT_OK` - the constructs (message class, phrase templates and logical response rule) were successfully created or formLinkAddr contains incorrect data.
* `SC_RESULT_ERROR`- internal error.