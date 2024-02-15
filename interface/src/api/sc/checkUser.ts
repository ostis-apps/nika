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

export const getFontSizeFromSettings = (value: string, par?: String) => {
    if (par == 'h') {
        if (value == 'small') return '10%';
        if (value == 'medium') return '120%';
        return '170%';
    }

    if (value == 'small') return '80%';
    if (value == 'medium') return '100%';
    return '150%';
};

export const translateWord = (text: string, lang: string) => {
    if (lang == 'ru' || lang == 'ru-RU') return text;
    if (lang == 'en' || lang == 'en-US') {
        text = text.toLocaleLowerCase();
        if (text == 'регистрация') return 'Sign up';
        if (text == 'войти') return 'Sign in';
        if (text == 'email') return 'Email';
        if (text == 'пароль') return 'Password';
        if (text == 'повторите пароль') return 'Repeat password';
        if (text == 'создать') return 'Create';
        if (text == 'уже есть аккаунт?') return 'Already have an account?';
        if (text == 'еще нет аккаунта?') return "Don't have an account yet?";
        if (text == 'здравствуйте') return 'Hello!';
        if (text == 'имя') return 'Name';
        if (text == 'чат') return 'Chat';
        if (text == 'развлечения') return 'Attractions';
        if (text == 'сохраненное') return 'Saved';
        if (text == 'назад') return 'Back';
        if (text == 'настройки') return 'Settings';
        if (text == 'тема') return 'Theme';
        if (text == 'поля должны быть заполнены!') return 'The fields must be filled in!';
        if (text == 'размер текста') return 'Font size';
        if (text == 'цвет интерфейса') return 'Accent color';
        if (text == 'сохранить') return 'Save';
        if (text == 'темная') return 'Dark';
        if (text == 'светлая') return 'Light';
        if (text == 'мелкий') return 'Little';
        if (text == 'средний') return 'Midlle';
        if (text == 'неверное имя пользователя или пароль!') return 'Invalid username or password!';
        if (text == 'язык') return 'Language';
        if (text == 'большой') return 'Big';
        if (text == 'имя пользователя уже занято!') return 'The username is already taken!';
        if (text == 'пароль должен состоять как минимум из 6 символов.')
            return 'The password must consist of at least 6 characters.';
        if (text == 'пароли не совпадают!') return "Passwords don't match!";
        if (text == 'имя пользователя должно состоять как минимум из 4 символов.')
            return 'The user name must consist of at least 4 characters.';
        if (text == 'Что умеет Рагнеда?') return 'What Ragneda can do?';

        if (text == 'отель') return 'hotel';
        if (text == 'парк') return 'park';
        if (text == 'церковь') return 'church';
        if (text == 'замок') return 'palace';
        if (text == 'аквапарк') return 'water park';
        if (text == 'бар') return 'bar';
        if (text == 'супермаркет') return 'supermarket';
        if (text == 'магазин') return 'shop';
        if (text == 'галерея') return 'galary';
        if (text == 'зоопарк') return 'zoo';
        if (text == 'бассейн') return 'pool';
        if (text == 'океанариум') return 'oceanarium';
        if (text == 'заповедники') return 'reserve';
        if (text == 'торговый центр') return 'mall';
        if (text == 'колесо обозрения') return 'ferris wheel';
        if (text == 'скульптура') return 'sculpture';
        if (text == 'усадьба') return 'manor house';
    }
};
