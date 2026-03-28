# Create author agent

This agent generates an author node with its credentials in the knowledge base.

**Action class:**

`action_create_google_author`

**Parameters:**

1. Browser session;
2. Code obtained from OAuth2.0 service.

**Agent workflow:**

* The agent uses the code to send a request to the OAuth2.0 service to obtain user metadata (name and email address).

* The agent generates an author node (an element of the concept_user class).

* The [token generation agent](./createTokensAgent.md) is called.

* After receiving the metadata, it is linked to the author node along with the browser session in sc-memory.

* The agent generates a dialogue node (an element of the concept_dialogue class) and links the author to it.

* The agent completes its work.

## Example

Example of input structure:

![input structure](./images/createAuthorStart.png)

Resulting structure of the agent's work:

![Resulting structure](./images/createAuthorEnd.png)

## Result

Possible results:

* `SC_RESULT_OK` - author generated;
* `SC_RESULT_ERROR` - internal error.
