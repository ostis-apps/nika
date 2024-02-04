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
    opacity,
    overflow,
} from './styled';
import { client } from '@api';
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { ScTemplate, ScType } from 'ts-sc-client';
import { Redirect } from 'react-router';
import { checkUser } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import { getUserName } from '@api/sc/checkUser';
import Alert from 'react-bootstrap';

type DesireDesc = {
    title: string;
    img: string;
    desireAddr: ScAddr;
    isSelected: boolean;
};

export const Intro = () => {
    // Get Cookies
    const cookie = new Cookie();
    const userAddr = cookie.get('userAddr') ? new ScAddr(parseInt(String(cookie.get('userAddr')))) : new ScAddr(0);
    const password = cookie.get('password');

    const [redirectError, setRedirectError] = useState<boolean>(false);
    const [savedDesires, setSavedDesires] = useState<boolean>(false);
    const [errorNameUserEmpty, setErrorNameUserEmpty] = useState<boolean>(false);
    const [desiresCountError, setDesiresCountError] = useState<boolean>(false);
    const [desireImages, setDesireImages] = useState<String[]>([]);

    const [nameUser, setNameUser] = useState<string>('');

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
    const [desiresCount, setDesiresCount] = useState<number>(0);

    const [, forceUpdate] = useReducer((x) => x + 1, 0);

    const findKb = async () => {
        let desireImages: String[] = [];

        const baseKeynodes = [
            { id: 'concept_infrastructure', type: ScType.NodeConstClass },
            { id: 'lang_ru', type: ScType.NodeConstClass },
        ];

        const helpKeynodes = [
            { id: 'nrel_inclusion', type: ScType.NodeConstNoRole },
            { id: 'nrel_main_idtf', type: ScType.NodeConstNoRole },
            { id: 'nrel_image', type: ScType.NodeConstNoRole },
        ];
        const keynodes = await client.resolveKeynodes(baseKeynodes);
        const hKeynodes = await client.resolveKeynodes(helpKeynodes);

        const template = new ScTemplate();
        template.tripleWithRelation(
            keynodes['concept_infrastructure'],
            ScType.EdgeDCommonVar,
            [ScType.NodeVarClass, '_type_desires'],
            ScType.EdgeAccessVarPosPerm,
            hKeynodes['nrel_inclusion'],
        );
        template.triple('_type_desires', ScType.EdgeAccessVarPosPerm, [ScType.NodeVarClass, '_desire']);
        template.tripleWithRelation(
            '_desire',
            ScType.EdgeDCommonVar,
            [ScType.LinkVar, '_desire_text'],
            ScType.EdgeAccessVarPosPerm,
            hKeynodes['nrel_main_idtf'],
        );
        template.tripleWithRelation(
            '_desire',
            ScType.EdgeDCommonVar,
            [ScType.LinkVar, 'desireImage'],
            ScType.EdgeAccessVarPosPerm,
            hKeynodes['nrel_image'],
        );

        template.triple(keynodes['lang_ru'], ScType.EdgeAccessVarPosPerm, '_desire_text');
        const result = await client.templateSearch(template);

        let a: DesireDesc[] = [];
        for (let i = 0; i < result.length; i++) {
            const linkTitle = result[i].get('_desire_text');
            const resTitle = await client.getLinkContents([linkTitle]);
            const title = String(resTitle[0].data);
            desireImages.push(String((await client.getLinkContents([result[i].get('desireImage')]))[0].data));

            console.log(await client.getLinkContents([result[i].get('desireImage')]));

            const desireAddr = result[i].get('_desire');
            const desireObj: DesireDesc = { title: title, img: '', desireAddr: desireAddr, isSelected: false };
            a.push(desireObj);
        }
        setDesires(a);
        setDesireImages(desireImages);
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
        if (desires[index].isSelected) setDesiresCount(desiresCount + 1);
        else setDesiresCount(desiresCount - 1);

        forceUpdate();
    };

    const changeUserName = (e: ChangeEvent<HTMLInputElement>) => {
        setNameUser(e.target.value);
    };

    const saveDesires = async (e) => {
        e.preventDefault();
        setDesiresCountError(false);
        setErrorNameUserEmpty(false);
        console.log(desiresCount);
        if (nameUser != '') {
            if (desiresCount < 7) {
                setDesiresCountError(true);
                return;
            }
            const construction = new ScConstruction();
            const baseKeynodes = [
                { id: 'nrel_desires', type: ScType.NodeConstNoRole },
                { id: 'nrel_name', type: ScType.NodeConstNoRole },
            ];
            const keynodes = await client.resolveKeynodes(baseKeynodes);

            construction.createNode(ScType.NodeConst, 'desires');
            for (let i = 0; i < desires.length; i++)
                if (desires[i].isSelected) {
                    console.log(desires[i].desireAddr);
                    construction.createEdge(ScType.EdgeAccessConstPosPerm, 'desires', desires[i].desireAddr);
                }

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
            setErrorNameUserEmpty(true);
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
                    </div>
                    <MainBtnsIntro>
                        {desires.map((item, index) => {
                            return (
                                <DesireButton
                                    style={{
                                        backgroundImage: `url(data:image/jpeg;base64,${desireImages[index]}) `,
                                        backgroundSize: 'cover',
                                    }}
                                    key={index}
                                    onClick={() => select(item, index)}
                                >
                                    <TextButton>{item.title}</TextButton>
                                    <LinerBtns></LinerBtns>
                                    <SelectMask style={item.isSelected ? opacity : undefined}>✓</SelectMask>
                                </DesireButton>
                            );
                        })}
                    </MainBtnsIntro>
                    {errorNameUserEmpty ? <Error>Поле должно быть заполнено!</Error> : ''}
                    {desiresCountError ? <Error>Выберете больше достопрмечательностей!</Error> : ''}
                    <SaveButton onClick={saveDesires}>Сохранить</SaveButton>
                </WrapperContentIntro>
            </IntroWrapper>
        </div>
    );
};
