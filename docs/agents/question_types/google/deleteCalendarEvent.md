# Google Calendar Event Deleting Agent

This agent deletes an event from the calendar based on characteristics obtained from the classifier.

**Action class:**

`action_delete_google_calendar_event`

**Parameters:**

1. Message node;
2. Authorized user node.

The logical rule that activates the agent is shown below.

![Message parameters](./images/lr_delete_event_message.png)

Required event parameters in the message:

- title.

**Agent workflow:**

- The agent searches for all event parameters.
- The agent finds the access token of the message author.
- The agent sends a request to the Google Calendar API to delete the event with the found parameters using the access token.
- The agent completes its work.

## Example

Example input structure:

![Input structure](./images/delete_event_start.png)

If the agent executes successfully, the first found event with the specified characteristics will be deleted from the author's calendar.

## Result

Possible results:

- `SC_RESULT_OK` - event deleted successfully;
- `SC_RESULT_ERROR` - internal error.
