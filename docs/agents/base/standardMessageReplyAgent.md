# Standard message reply agent

Is an agent that generates a reply message based on existing rules in the knowledge base.
The agent uses phrase generation and direct inference agents.

At first the StandardMessageReplyAgent generates the reply message construction.
Gathers logic rules and a target template, then forwards information to Direct Inference Agent (from scl-machine) for further processing. You can find out more about DirectInferenceAgent in the documentation of [scl-machine](../subsystems/scl-machine.md).
Then it calls the PhraseGenerationAgent to generate a sc-link with the reply message text.

**Action class:**

`action_standard_message_reply`

**Parameters:**

1. `messageAddr` -- an element of `concept_message` and `concept_atomic_message` or `concept_non_atomic_message`.

### Example

#### 1. Generation of an atomic message

1.1. Example of an input structure:

<img src="../images/standardMessageReplyAgentAtomicInput.png"></img>

1.2. Example of a logic rule:

<img src="../images/standardMessageReplyAgentAtomicMessageRule.png"></img>

1.3. Example of a phrase:

<img src="../images/standardMessageReplyAgentAtomicPhrase.png"></img>

1.4. Example of an output structure (an atomic message):

<img src="../images/standardMessageReplyAgentAtomicMessageOutput.png"></img>

#### 2. Generation of a non-atomic message

2.1. Example of an input structure:

<img src="../images/standardMessageReplyAgentNonAtomicInput.png"></img>

2.2. Example of a logic rule:

<img src="../images/standardMessageReplyAgentNonAtomicMessageRule.png"></img>

2.3. Example of phrases:

<img src="../images/standardMessageReplyAgentNonAtomicPhrase.png"></img>

2.4. Example of an output structure (a non-atomic message)

<img src="../images/standardMessageReplyAgentNonAtomicMessageOutput.png"></img>

### Result

Possible result codes:

* `SC_RESULT_OK` - the reply message is generated.
* `SC_RESULT_ERROR` - internal error.
* `SC_RESULT_ERROR_invalid_params` - the action has no incoming message.
