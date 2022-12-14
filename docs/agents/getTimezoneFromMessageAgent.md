Is an agent that extracts timezone information from message if timezone is an element of a message with relation 'rrel_entity'

**Action class:**

`action_get_timezone_from_message`


**Parameters:**

1. `message`
2. `timezone` - set that will hold timezone.

**Workflow:**

* The agent checks whether a message belongs to a 'concept_message_about_time_in_timezone' or not. If not, execution stops.
* The agent searches for a timezone that is contained by a message with relation 'rrel_entity'
* Then timezone is added to a second parameter as set element.

### Example

Example of an input structure:

<img src="../images/getTimezoneFromMessageAgentInput.png"></img>

Example of an output structure:

<img src="../images/getTimezoneFromMessageAgentOutput.png"></img>

### Agent implementation language

C++

### Result

Possible result codes:
 
* `sc_result_ok` - timezone is added to a second parameter set;
* `sc_result_error`- internal error.
