import { ScTemplate, ScType, ScAddr } from 'ts-sc-client';
import { client } from '@api/sc';
import Cookie from 'universal-cookie';

const conceptUser = 'concept_user';
const conceptDialog = 'concept_dialogue';
const rrelDialogParticipant = 'rrel_dialog_participant';

const baseKeynodes = [
    { id: conceptUser, type: ScType.NodeConstClass },
    { id: conceptDialog, type: ScType.NodeConstClass },
    { id: rrelDialogParticipant, type: ScType.NodeConstRole },
];

const getUser = async (userAddr: ScAddr) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const dialog = '_dialog';

    const template = new ScTemplate();
    template.triple(keynodes[conceptUser], ScType.EdgeAccessVarPosPerm, userAddr);
    template.triple(keynodes[conceptDialog], ScType.EdgeAccessVarPosPerm, [ScType.NodeVar, dialog]);
    template.tripleWithRelation(
        dialog,
        ScType.EdgeAccessVarPosPerm,
        userAddr,
        ScType.EdgeAccessVarPosPerm,
        keynodes[rrelDialogParticipant],
    );
    const result = await client.templateSearch(template, {});
    if (result.length > 0) return userAddr;
    return null;
};

const createUser = async (userAddr: ScAddr) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const dialog = '_dialog';

    const template = new ScTemplate();
    template.triple(keynodes[conceptUser], ScType.EdgeAccessVarPosPerm, userAddr);
    template.triple(keynodes[conceptDialog], ScType.EdgeAccessVarPosPerm, [ScType.NodeVar, dialog]);
    template.tripleWithRelation(
        dialog,
        ScType.EdgeAccessVarPosPerm,
        userAddr,
        ScType.EdgeAccessVarPosPerm,
        keynodes[rrelDialogParticipant],
    );
    const result = await client.templateGenerate(template, {});
    return userAddr;
};

export const resolveUserAgent = async () => {
    const cookie = new Cookie();
    const userAddr = new ScAddr(parseInt(cookie.get('userAddr')));

    const user = await getUser(userAddr);
    console.log(user);
    if (user !== null) {
        console.log(user);
        return user;
    }
    console.log('start createUser');
    return await createUser(userAddr);
};
