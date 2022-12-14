Is an agent that removes information about previously resolved date and time information for a timezone

**Action class:**

`action_remove_timezone_date_time`


**Parameters:**

1. `timezone set` - set that holds timezone.

**Workflow:**

* The agent gets timezone from timezone set.
* The agent removes information about previously resolved date and time.

### Example

Example of an input structure:

<img src="../images/removeTimezoneDateTimeAgentInput.png"></img>

Example of an output structure:

<img src="../images/removeTimezoneDateTimeAgentOutput.png"></img>

### Agent implementation language

C++

### Result

Possible result codes:
 
* `sc_result_ok` - timezone's previous date and time information was removed;
* `sc_result_error`- internal error.
