import { client } from '@api';
import Settings from '@pages/Settings';
import { ScrollZoomHandler } from 'mapbox-gl';
import { ScAddr, ScConstruction, ScHelper, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
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

export const checkEmail = async (username: string) => {
    const baseKeynodes = [{ id: 'nrel_email', type: ScType.NodeConstNoRole }];
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const template = new ScTemplate();
    const userNameLink = (await client.getLinksByContents([username]))[0][0];

    template.tripleWithRelation(
        ScType.NodeVar,
        ScType.EdgeDCommonVar,
        userNameLink,
        ScType.EdgeAccessVarPosPerm,
        keynodes['nrel_email'],
    );
    const result = await client.templateSearch(template);
    if (result.length > 0) {
        console.log(1);
        return true;
    }
    return false;
};

export const findSettings = async (userAddr: ScAddr, settingName: string) => {
    const baseKeynodes = [
        { id: settingName, type: ScType.NodeConstNoRole },
        { id: 'nrel_user_settings', type: ScType.NodeConstNoRole },
    ];
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const template = new ScTemplate();
    template.tripleWithRelation(
        userAddr,
        ScType.EdgeDCommonVar,
        [ScType.NodeVar, '_settings'],
        ScType.EdgeAccessVarPosPerm,
        keynodes['nrel_user_settings'],
    );
    template.tripleWithRelation(
        '_settings',
        ScType.EdgeDCommonVar,
        [ScType.LinkVar, '_setting_value'],
        ScType.EdgeAccessVarPosPerm,
        keynodes[settingName],
    );

    const result = await client.templateSearch(template);
    if (result.length > 0) {
        const ans = String((await client.getLinkContents([result[0].get('_setting_value')]))[0].data);
        return ans;
    }
    return '';
};

export const updateSettings = async (userAddr: ScAddr, settingName: string, value: string) => {
    const baseKeynodes = [
        { id: settingName, type: ScType.NodeConstNoRole },
        { id: 'nrel_user_settings', type: ScType.NodeConstNoRole },
    ];
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const template = new ScTemplate();
    template.tripleWithRelation(
        userAddr,
        ScType.EdgeDCommonVar,
        [ScType.NodeVar, '_settings'],
        ScType.EdgeAccessVarPosPerm,
        keynodes['nrel_user_settings'],
    );
    template.tripleWithRelation(
        '_settings',
        ScType.EdgeDCommonVar,
        [ScType.LinkVar, '_setting_value'],
        ScType.EdgeAccessVarPosPerm,
        keynodes[settingName],
    );

    const result = await client.templateSearch(template);
    if (result.length > 0) {
        console.log(value);
        const construction = new ScLinkContent(value, ScLinkContentType.String, result[0].get('_setting_value'));
        const res = await client.setLinkContents([construction]);
        console.log(1);
        return;
    }
    console.log(1);
    return '';
};

export const getUserSettings = async (userAddr: ScAddr) => {
    const baseKeynodes = [
        { id: 'nrel_user_settings', type: ScType.NodeConstNoRole },
        { id: 'nrel_user_settings', type: ScType.NodeConstNoRole },
    ];
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const template = new ScTemplate();
    template.tripleWithRelation(
        userAddr,
        ScType.EdgeDCommonVar,
        [ScType.NodeVar, '_settings'],
        ScType.EdgeAccessVarPosPerm,
        keynodes['nrel_user_settings'],
    );
    template.tripleWithRelation(
        '_settings',
        ScType.EdgeDCommonVar,
        [ScType.LinkVar, '_setting_value'],
        ScType.EdgeAccessVarPosPerm,
        [ScType.NodeVar, '_setting_rel'],
    );

    const result = await client.templateSearch(template);
    if (result.length > 0) {
        let settings = {};
        for (let i = 0; i < result.length; i++) {
            const healper = new ScHelper(client);
            const idtf = String(await healper.getSystemIdentifier(result[i].get('_setting_rel')));
            settings[idtf] = String((await client.getLinkContents([result[i].get('_setting_value')]))[0].data);
        }
        return settings;
    }
    return '';
};

export const getFontSize = (value: string) => {
    if (value == 'little') return;
    if (value == 'medium') return;
    return;
};
