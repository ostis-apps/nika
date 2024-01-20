import { client } from "@api";
import { ScAddr } from 'ts-sc-client';
import { ScTemplate, ScType } from 'ts-sc-client';


export const checkUser = async (userAddr:ScAddr, password:string) => {
    const baseKeynodes = [
        { id: "nrel_password", type: ScType.NodeConstNoRole },
    ];
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const template = new ScTemplate();
    template.tripleWithRelation(
        userAddr,
        ScType.EdgeDCommonVar,
        ScType.LinkVar,
        ScType.EdgeAccessVarPosPerm,
        [keynodes['nrel_password'], '_password'],
    )
    const result = await client.templateSearch(template);

    if (result.length > 0)
        return (await client.getLinkContents([result[0].get('_password')]))[0].data === password ? true : false;
}