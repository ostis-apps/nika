# Send Mail Agent

This agent sends an email using the SMTP protocol.

**Action class:**

`action_send_google_mail`

**Parameters:**

1. Message node;
2. Authorized user node.

The logical rule that activates the agent is shown below.

![Message parameters](./images/send_mail.png)

The message parameters must specify the text of the email to be sent and either an email address or a contact name to send it to.

**Agent workflow:**

- The agent searches for all message parameters.
- The agent finds the email address of the message author.
- The agent sends the message via the SMTP protocol;
- The agent completes its work.

## Example

Example input structure:

![Input structure](./images/send_mail_start.bmp)

If the agent executes successfully, a message will be sent on behalf of the author to the specified address.

## Result

Possible results:

- `SC_RESULT_OK` - message sent successfully;
- `SC_RESULT_ERROR` - internal error.