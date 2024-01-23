import React, { useEffect, useState, useReducer, ChangeEvent } from 'react';
import {
    BackgroundCircle,
    IntroWrapper,
    WrapperContentIntro,
    HelloTextIntro,
    NameInput,
    MainBtnsIntro,
    DesireButton,
    LinerBtns,
    TextButton,
    SelectMask,
    SaveButton,
    Error,
} from './styled';
import { client } from '@api';
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { ScTemplate, ScType, ScEventType } from 'ts-sc-client';
import { Redirect } from 'react-router';
import { checkUser } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import { getUserName } from '@api/sc/checkUser';

type DesireDesc = {
    title: string;
    img: string;
    desireAddr: ScAddr;
    isSelected: boolean;
};

export const Intro = () => {
    const cookie = new Cookie();

    const [nameUser, setNameUser] = useState<string>('');
    const [checkUserName, setCheckUserName] = useState<string>('');
    const [redirectError, setRedirectError] = useState<boolean>(false);
    const [savedDesires, setSavedDesires] = useState<boolean>(false);
    const [nameUserEmpty, setErrorUserEmpty] = useState<boolean>(false);

    const userAddr = cookie.get('userAddr') ? new ScAddr(parseInt(String(cookie.get('userAddr')))) : new ScAddr(0);
    const password = cookie.get('password');

    const check = async () => {
        if (userAddr.isValid() && password) {
            if (!(await checkUser(userAddr, password))) {
                setRedirectError(true);
            } else if (await getUserName(userAddr)) {
                setSavedDesires(true);
            }
        } else {
            setRedirectError(true);
        }
    };

    const [desires, setDesires] = useState<DesireDesc[]>([]);
    const [, forceUpdate] = useReducer((x) => x + 1, 0);

    const findKb = async () => {
        const conceptInfrastructure = 'concept_infrastructure';
        const langRu = 'lang_ru';
        const componentInclusion = 'nrel_inclusion';
        const componentNrelMainIdtf = 'nrel_main_idtf';

        const baseKeynodes = [
            { id: conceptInfrastructure, type: ScType.NodeConstClass },
            { id: langRu, type: ScType.NodeConstClass },
        ];

        const helpKeynodes = [
            { id: componentInclusion, type: ScType.NodeConstNoRole },
            { id: componentNrelMainIdtf, type: ScType.NodeConstNoRole },
        ];
        const keynodes = await client.resolveKeynodes(baseKeynodes);
        const hKeynodes = await client.resolveKeynodes(helpKeynodes);

        const typeDesiresClass = '_type_desires';
        const desire = '_desire';
        const desireText = '_desire_text';

        const template = new ScTemplate();
        template.tripleWithRelation(
            keynodes[conceptInfrastructure],
            ScType.EdgeDCommonVar,
            [ScType.NodeVarClass, typeDesiresClass],
            ScType.EdgeAccessVarPosPerm,
            hKeynodes[componentInclusion],
        );
        template.triple(typeDesiresClass, ScType.EdgeAccessVarPosPerm, [ScType.NodeVarClass, desire]);
        template.tripleWithRelation(
            desire,
            ScType.EdgeDCommonVar,
            [ScType.LinkVar, desireText],
            ScType.EdgeAccessVarPosPerm,
            hKeynodes[componentNrelMainIdtf],
        );
        template.triple(keynodes[langRu], ScType.EdgeAccessVarPosPerm, desireText);
        const result = await client.templateSearch(template);

        let a: DesireDesc[] = [];
        for (let i = 0; i < result.length; i++) {
            const linkTitle = result[i].get(desireText);
            const resTitle = await client.getLinkContents([linkTitle]);
            const title = String(resTitle[0].data);

            const desireAddr = result[i].get(desire);
            const desireObj: DesireDesc = { title: title, img: '', desireAddr: desireAddr, isSelected: false };
            a.push(desireObj);
        }
        setDesires(a);
    };

    useEffect(() => {
        (async () => {
            check();
            findKb();
        })();
    }, []);

    useEffect(() => {
        (async () => {
            forceUpdate();
        })();
    }, [desires]);

    const select = (item, index) => {
        desires[index].isSelected = desires[index].isSelected ? false : true;
        forceUpdate();
    };

    const opacity = {
        opacity: '1',
    };

    const overflow = {
        overflow: 'auto',
    };

    const changeUserName = (e: ChangeEvent<HTMLInputElement>) => {
        setNameUser(e.target.value);
    };

    const saveDesires = async (e) => {
        e.preventDefault();
        if (nameUser != '') {
            const construction = new ScConstruction();
            const baseKeynodes = [
                { id: 'nrel_desires', type: ScType.NodeConstNoRole },
                { id: 'nrel_name', type: ScType.NodeConstNoRole },
            ];
            const keynodes = await client.resolveKeynodes(baseKeynodes);

            construction.createNode(ScType.NodeConst, 'desires');
            for (let i = 0; i < desires.length; i++)
                if (desires[i].isSelected)
                    construction.createEdge(ScType.EdgeAccessConstPosPerm, 'desires', desires[i].desireAddr);

            construction.createEdge(ScType.EdgeDCommonConst, userAddr, 'desires', 'desiresEdge');
            construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['nrel_desires'], 'desiresEdge');
            construction.createLink(
                ScType.LinkConst,
                new ScLinkContent(nameUser, ScLinkContentType.String),
                'nameLink',
            );
            construction.createEdge(ScType.EdgeDCommonConst, userAddr, 'nameLink', 'nameEdge');
            construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['nrel_name'], 'nameEdge');

            await client.createElements(construction);

            setSavedDesires(true);
        } else {
            setErrorUserEmpty(true);
        }
    };

    return (
        <div style={overflow}>
            {savedDesires ? <Redirect to={{ pathname: routes.HOME }} /> : ''}
            {redirectError ? <Redirect to={{ pathname: routes.LOGIN }} /> : ''}
            <BackgroundCircle />
            <IntroWrapper>
                <WrapperContentIntro>
                    <div>
                        <HelloTextIntro>Здравствуйте</HelloTextIntro>
                        <NameInput type="text" placeholder="Имя" onChange={changeUserName}></NameInput>
                        {nameUserEmpty ? <Error>Поле должно быть заполнено!</Error> : ''}
                    </div>
                    <MainBtnsIntro>
                        {desires.map((item, index) => {
                            return (
                                <DesireButton key={index} onClick={() => select(item, index)}>
                                    <TextButton>{item.title}</TextButton>
                                    <LinerBtns></LinerBtns>
                                    <SelectMask style={item.isSelected ? opacity : undefined}>✓</SelectMask>
                                </DesireButton>
                            );
                        })}
                    </MainBtnsIntro>
                    <SaveButton onClick={saveDesires}>Сохранить</SaveButton>
                </WrapperContentIntro>
            </IntroWrapper>
        </div>
    );
};
