import {ScTemplate, ScType} from 'ts-sc-client';
import { client } from '@api/sc';

const conceptUser = 'concept_user';
const conceptDialog = 'concept_dialogue';
const rrelDialogParticipant = 'rrel_dialog_participant';

const baseKeynodes = [
    { id: conceptUser, type: ScType.NodeConstClass },
    { id: conceptDialog, type: ScType.NodeConstClass },
    { id: rrelDialogParticipant, type: ScType.NodeConstRole },
];

const getUser = async () => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const user = '_user';

    const template = new ScTemplate();
    template.triple(
        keynodes[conceptUser],
        ScType.EdgeAccessVarPosPerm,
        [ScType.NodeVar, user],
    );
    const result = await client.templateSearch(template);
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
        ScType.EdgeAccessVarPosPerm,
        [ScType.NodeVar, user],
    );
    template.triple(
        keynodes[conceptDialog],
        ScType.EdgeAccessVarPosPerm,
        [ScType.NodeVar, dialog],
    );
    template.tripleWithRelation(
        dialog,
        ScType.EdgeAccessVarPosPerm,
        user,
        ScType.EdgeAccessVarPosPerm,
        keynodes[rrelDialogParticipant],
    );
    const result = await client.templateGenerate(template, {});
    return result?.get(user);
}

export const resolveUserAgent = async () => {
    const user = await getUser();
    if (user !== null) {
        return user;
    }
    return await createUser();
};
