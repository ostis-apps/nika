import { ScAddr, ScConstruction, ScEventSubscriptionParams, ScEventType, ScTemplate, ScType } from 'ts-sc-client';
import { client } from '@api/sc/client';

const action = 'action';
const actionInitiated = 'action_initiated';
const answer = 'nrel_answer';
const actionFinished = 'action_finished';

const baseKeynodes = [
    { id: action, type: ScType.NodeConstClass },
    { id: actionInitiated, type: ScType.NodeConstClass },
    { id: answer, type: ScType.NodeConstNoRole },
    { id: actionFinished, type: ScType.NodeConstClass },
];

const describeAgent = async (template: ScTemplate, actionNodeAlias: string) => {
    const generationResult = await client.templateGenerate(template, {});

    if (generationResult && generationResult.size > 0) {
        return generationResult.get(actionNodeAlias);
    }
    return null;
};

const findResultCircuit = async (actionNode: ScAddr, keynodes: Record<string, ScAddr>) => {
    const circuitDialogAlias = '_circuit_dialog';
    const template = new ScTemplate();

    template.tripleWithRelation(
        actionNode,
        ScType.EdgeDCommonVar,
        [ScType.NodeVarStruct, circuitDialogAlias],
        ScType.EdgeAccessVarPosPerm,
        keynodes[answer],
    );
    const result = await client.templateSearch(template);

    if (result.length) {
        return result[0].get(circuitDialogAlias);
    }
    return null;
};

const subscribeToAgentAnswer = async (actionNode: ScAddr, keynodes: Record<string, ScAddr>, onResponse: () => void) => {
    const onActionFinished = (_subscibedAddr: ScAddr, _arc: ScAddr, anotherAddr: ScAddr, eventId: number) => {
        if (anotherAddr.isValid() && anotherAddr.equal(keynodes[actionFinished])) {
            client.eventsDestroy(eventId);
            onResponse();
        }
    };

    const eventParams = new ScEventSubscriptionParams(actionNode, ScEventType.AddIngoingEdge, onActionFinished);

    client.eventsCreate(eventParams);
};

export const makeAgent = (template: ScTemplate, actionNodeAlias: string) => {
    return new Promise<ScAddr | null>((resolve) => {
        client.resolveKeynodes(baseKeynodes).then(async (keynodes) => {
            const actionNode = await describeAgent(template, actionNodeAlias);
            if (!actionNode) return resolve(null);

            const onResponse = async () => {
                resolve(await findResultCircuit(actionNode, keynodes));
            };
            await subscribeToAgentAnswer(actionNode, keynodes, onResponse);
            const construction = new ScConstruction();
            construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes[actionInitiated], actionNode);
            client.createElements(construction);
        });
    });
};
