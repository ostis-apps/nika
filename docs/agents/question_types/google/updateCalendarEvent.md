# Google Calendar Event Updating Agent

This agent updates an event in the calendar based on characteristics obtained from the classifier.

**Action class:**

`action_update_google_calendar_event`

**Parameters:**

1. Message node;
2. Authorized user node.

The logical rule that activates the agent is shown below.

![Message parameters](./images/lr_update_event_message.png)

Required parameters of the old event in the message:

- title.

New event parameters that can be specified:

- title;
- start time (UTC format);
- end time (UTC format).

**Agent workflow:**

- The agent searches for all event parameters.
- The agent finds the access token of the message author.
- The agent sends a request to the Google Calendar API to update the event with the found parameters using the access token.
- The agent completes its work.

## Example

Example input structure:

![Input structure](./images/update_start.png)

If the agent executes successfully, the event characteristics in the author's calendar will be updated.

## Result

Possible results:

- `SC_RESULT_OK` - event updated successfully;
- `SC_RESULT_ERROR` - internal error.
