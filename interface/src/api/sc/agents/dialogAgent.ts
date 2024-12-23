import { ScAddr, ScTemplate, ScType } from 'ts-sc-client';
import { client } from '@api/sc/client';

const conceptDialog = 'concept_dialogue';
const rrelDialogParticipant = 'rrel_dialog_participant';

const baseKeynodes = [
    { id: conceptDialog, type: ScType.ConstNodeClass },
    { id: rrelDialogParticipant, type: ScType.ConstNodeRole },
];

const findDialogNode = async (user: ScAddr) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const dialog = '_dialog';
    const template = new ScTemplate();
    template.triple(
        keynodes[conceptDialog],
        ScType.VarPermPosArc,
        [ScType.VarNode, dialog],
    );
    template.quintuple(
        dialog,
        ScType.VarPermPosArc,
        user,
        ScType.VarPermPosArc,
        keynodes[rrelDialogParticipant],
    );
    const resultDialogNode = await client.searchByTemplate(template);

    if (resultDialogNode.length) {
        return resultDialogNode[0].get(dialog);
    }

    return null;
};

export const dialogAgent = async (users: ScAddr[]) => {
    return await findDialogNode(users[0]);
};
