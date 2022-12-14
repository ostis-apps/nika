Is an agent that requests date and time for a timezone from web service

**Action class:**

`action_add_timezone_date_time`


**Parameters:**

1. `timezone set` - set that holds timezone.

**Workflow:**

* The agent gets timezone from timezone set.
* The agent removes outdated information about previously resolved date and time.
* The agent requests current date and time information from service and adds it to a timezone using constant positive temporal access edge between `nrel_date` and `nrel_time` and links that represent current date and current time.

### Example

Example of an input structure:

<img src="../images/addTimezoneDateTimeAgentInput.png"></img>

Example of an output structure:

<img src="../images/addTimezoneDateTimeAgentOutput.png"></img>

### Agent implementation language

C++

### Result

Possible result codes:
 
* `sc_result_ok` - timezone's date and time resolved successfully;
* `sc_result_error`- internal error.
