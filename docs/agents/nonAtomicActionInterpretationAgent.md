Is an agent that generates an action description in the knowlendge base based on the template received. If there are sets of arguments then it performs a role mapping before generation.

**Action class:**

`action_interpret_non_atomic_action`

**Parameters:**

1. `nonAtomicActionTemplateAddr` containing a description of a non-atomic action;
2. **(optional)** `argumentsSet` -- action argument set.


**Comment:**

* If there is no need for a `set of arguments`, then an empty `set of arguments` or no set is allowed;
* The transition to the next action depends on the result of the previous one.


**Workflow:**

* The first argument of the non-atomic action is replaced by the node that belongs to the set of arguments as the first, the second argument of the non-atomic action is replaced by the node belongs to the set as the second;
* In the course of an agent's work, the `template` program generates a description of a non-atomic action. The atomic actions composing it are also added to the class of performed (successfully or unsuccessfully) by the agents that performed them. For an example of an interpreted agent program, see below:

<img src="../images/nonAtomicActionInterpretationAgentExample.png"></img>

### Example

Example of an input structure:

<img src="../images/nonAtomicActionInterpretationAgentInput.png"></img>

Example of an output structure:

<img src="../images/nonAtomicActionInterpretationAgentOutput.png"></img>

### Result

Possible result codes:
 
* `SC_RESULT_OK`- interpretation completed;
* `SC_RESULT_ERROR`- internal error.
