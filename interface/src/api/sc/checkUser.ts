import { client } from '@api';
import { ScAddr } from 'ts-sc-client';
import { ScTemplate, ScType } from 'ts-sc-client';

export const checkUser = async (userAddr: ScAddr, password: string | undefined) => {
    const baseKeynodes = [{ id: 'nrel_password', type: ScType.NodeConstNoRole }];
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const template = new ScTemplate();
    template.tripleWithRelation(
        userAddr,
        ScType.EdgeDCommonVar,
        [ScType.LinkVar, '_password'],
        ScType.EdgeAccessVarPosPerm,
        keynodes['nrel_password'],
    );
    const result = await client.templateSearch(template);

    if (result.length > 0) {
        const pass1 = (await client.getLinkContents([result[0].get('_password')]))[0].data;
        return pass1 === password ? true : false;
    }
};

export const getUserName = async (userAddr) => {
    const baseKeynodes = [{ id: 'nrel_name', type: ScType.NodeConstNoRole }];
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const template = new ScTemplate();
    template.tripleWithRelation(
        userAddr,
        ScType.EdgeDCommonVar,
        [ScType.LinkVar, '_userName'],
        ScType.EdgeAccessVarPosPerm,
        keynodes['nrel_name'],
    );
    const result = await client.templateSearch(template);
    if (result.length > 0) {
        return String((await client.getLinkContents([result[0].get('_userName')]))[0].data);
    }
    return '';
};
