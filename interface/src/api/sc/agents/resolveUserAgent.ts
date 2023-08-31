import {ScTemplate, ScType} from 'ts-sc-client';
import { client } from '@api/sc';

const conceptUser = 'concept_user';
const conceptDialog = 'concept_dialogue';
const nrelDialogParticipant = 'nrel_dialog_participants';
const nika = 'nika';
const nrelDialogueHistory = 'nrel_dialogue_history';

const baseKeynodes = [
    { id: conceptUser, type: ScType.NodeConstClass },
    { id: conceptDialog, type: ScType.NodeConstClass },
    { id: nrelDialogParticipant, type: ScType.NodeConstNoRole },
    { id: nika, type: ScType.NodeConst },
    { id: nrelDialogueHistory, type: ScType.NodeConstNoRole },
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
    const tuple = '_tuple';
    const tupleUser = '_tuple_user';
    const tupleNika = '_tuple_nika';

    const template = new ScTemplate();
    template.triple(
        [ScType.NodeVarTuple, tuple],
        ScType.EdgeAccessVarPosPerm,
        [ScType.NodeVar, user],
    );
    template.triple(
        tuple,
        ScType.EdgeAccessVarPosPerm,
        keynodes[nika],
    );
    template.triple(
        keynodes[conceptUser],
        ScType.EdgeAccessVarPosPerm,
        user,
    );
    template.triple(
        keynodes[conceptDialog],
        ScType.EdgeAccessVarPosPerm,
        [ScType.NodeVar, dialog],
    );
    template.tripleWithRelation(
        tuple,
        ScType.EdgeDCommonVar,
        dialog,
        ScType.EdgeAccessVarPosPerm,
        keynodes[nrelDialogParticipant],
    );
    template.triple(
        [ScType.NodeVarTuple, tupleUser],
        ScType.EdgeAccessVarPosPerm,
        dialog,
    );
    template.triple(
        [ScType.NodeVarTuple, tupleNika],
        ScType.EdgeAccessVarPosPerm,
        dialog,
    );
    template.tripleWithRelation(
        tupleUser,
        ScType.EdgeDCommonVar,
        user,
        ScType.EdgeAccessVarPosPerm,
        keynodes[nrelDialogueHistory],
    );
    template.tripleWithRelation(
        tupleNika,
        ScType.EdgeDCommonVar,
        keynodes[nika],
        ScType.EdgeAccessVarPosPerm,
        keynodes[nrelDialogueHistory],
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
