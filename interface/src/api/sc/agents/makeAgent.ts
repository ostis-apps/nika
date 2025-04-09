import { ScAddr, ScConstruction, ScEventSubscriptionParams, ScEventType, ScTemplate, ScType } from 'ts-sc-client';
import { client } from '@api/sc/client';

const action = 'action';
const actionInitiated = 'action_initiated';
const result = 'nrel_result';
const actionFinished = 'action_finished';

const baseKeynodes = [
    { id: action, type: ScType.ConstNodeClass },
    { id: actionInitiated, type: ScType.ConstNodeClass },
    { id: result, type: ScType.ConstNodeNonRole },
    { id: actionFinished, type: ScType.ConstNodeClass },
];

const describeAgent = async (template: ScTemplate, actionNodeAlias: string) => {
    const generationResult = await client.generateByTemplate(template, {});

    if (generationResult && generationResult.size > 0) {
        return generationResult.get(actionNodeAlias);
    }
    return null;
};

const findResultCircuit = async (actionNode: ScAddr, keynodes: Record<string, ScAddr>) => {
    const circuitDialogAlias = '_circuit_dialog';
    const template = new ScTemplate();

    template.quintuple(
        actionNode,
        ScType.VarCommonArc,
        [ScType.VarNodeStructure, circuitDialogAlias],
        ScType.VarPermPosArc,
        keynodes[result],
    );
    const searchResult = await client.searchByTemplate(template);

    if (searchResult.length) {
        return searchResult[0].get(circuitDialogAlias);
    }
    return null;
};

const subscribeToAgentAnswer = async (actionNode: ScAddr, keynodes: Record<string, ScAddr>, onResponse: () => void) => {
    const onActionFinished = (_subscibedAddr: ScAddr, _arc: ScAddr, anotherAddr: ScAddr, eventId: number) => {
        if (anotherAddr.isValid() && anotherAddr.equal(keynodes[actionFinished])) {
            client.destroyElementaryEventSubscriptions(eventId);
            onResponse();
        }
    };

    const eventParams = new ScEventSubscriptionParams(actionNode, ScEventType.AfterGenerateIncomingArc, onActionFinished);

    client.createElementaryEventSubscriptions(eventParams);
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
            construction.generateConnector(ScType.ConstPermPosArc, keynodes[actionInitiated], actionNode);
            client.generateElements(construction);
        });
    });
};
