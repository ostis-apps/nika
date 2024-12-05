# Audio recognition agent

This is an agent that recognizes speech spoken by the user.

**Action class:**

`action_recognize_audio`

**Parameters:**

1. `messageAddr` -- an element of `concept_message`.

**Libraries used:**

* SpeechRecognition - to recognize speech.

**Comment:**

* Internet connection is required for the agent to work.

### Examples

Examples of an input structure:

<img src="../images/audioRecognitionAgentInput.png"></img>

Examples of an outnput structure:

<img src="../images/audioRecognitionAgentOutput.png"></img>

### Agent implementation language
Python

### Result

Possible result codes:

* `SC_RESULT_OK` - agent created a structure with recognized text.
* `SC_RESULT_ERROR`- internal error.
