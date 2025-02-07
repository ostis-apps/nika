import {ScTemplate, ScType} from 'ts-sc-client';
import { client } from '@api/sc';

const conceptUser = 'concept_user';
const conceptDialog = 'concept_dialogue';
const rrelDialogParticipant = 'rrel_dialog_participant';

const baseKeynodes = [
    { id: conceptUser, type: ScType.ConstNodeClass },
    { id: conceptDialog, type: ScType.ConstNodeClass },
    { id: rrelDialogParticipant, type: ScType.ConstNodeRole },
];

const getUser = async () => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const user = '_user';

    const template = new ScTemplate();
    template.triple(
        keynodes[conceptUser],
        ScType.VarPermPosArc,
        [ScType.VarNode, user],
    );
    const result = await client.searchByTemplate(template);
    if (result.length === 1) {
        return result[0].get(user);
    }
    return null;
}

const createUser = async () => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const user = '_user';
    const dialog = '_dialog';

    const template = new ScTemplate();
    template.triple(
        keynodes[conceptUser],
        ScType.VarPermPosArc,
        [ScType.VarNode, user],
    );
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
    const result = await client.generateByTemplate(template, {});
    return result?.get(user);
}

export const resolveUserAgent = async () => {
    const user = await getUser();
    if (user !== null) {
        return user;
    }
    return await createUser();
};
