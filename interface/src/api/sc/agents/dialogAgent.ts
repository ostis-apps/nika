import { ScAddr, ScTemplate, ScType } from 'ts-sc-client';
import { client } from '@api/sc/client';

const conceptDialog = 'concept_dialogue';
const nrelDialogParticipant = 'nrel_dialog_participants';

const baseKeynodes = [
    { id: conceptDialog, type: ScType.NodeConstClass },
    { id: nrelDialogParticipant, type: ScType.NodeConstNoRole },
];

const findDialogNode = async (user: ScAddr) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const dialog = '_dialog';
    const tuple = '_tuple';

    const template = new ScTemplate();
    template.triple(
        keynodes[conceptDialog],
        ScType.EdgeAccessVarPosPerm,
        [ScType.NodeVar, dialog],
    );
    template.triple(
        [ScType.NodeVarTuple, tuple],
        ScType.EdgeAccessVarPosPerm,
        user,
    );
    template.tripleWithRelation(
        tuple,
        ScType.EdgeDCommonVar,
        dialog,
        ScType.EdgeAccessVarPosPerm,
        keynodes[nrelDialogParticipant],
    );
    const resultDialogNode = await client.templateSearch(template);

    if (resultDialogNode.length) {
        return resultDialogNode[0].get(dialog);
    }

    return null;
};

export const dialogAgent = async (users: ScAddr[]) => {
    return await findDialogNode(users[0]);
};
