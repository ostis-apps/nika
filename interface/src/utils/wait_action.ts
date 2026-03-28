import { client } from '@api';
import { ScAddr, ScEventType, ScEventSubscriptionParams } from 'ts-sc-client';

export function waitForAction(actionNode: ScAddr): Promise<void> {
  return new Promise(async (resolve) => {
    // Create the subscription with a callback that resolves the Promise
    const subscription = new ScEventSubscriptionParams(
      actionNode,
      ScEventType.AfterGenerateIncomingArc,   // adjust to the actual finish event
      () => {
        resolve();                          // action finished → resolve
        // Optional: unsubscribe if the client requires manual cleanup
        // client.destroyEvents([subscription]);
      }
    );
    await client.createElementaryEventSubscriptions([subscription]);
    // Activate the subscription (method name may vary)
    // client.subscribeEvents([subscription]);
  });
}