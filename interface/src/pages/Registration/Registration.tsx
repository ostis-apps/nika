import React, { useEffect, useState, ChangeEvent } from 'react';
import { Circle, Circle1, Wrapper, WrapperContent, Form, FormText, Input, FormBtn, Error, Link } from './styled';
import { client } from '@api';
import { routes } from '@constants';
import {
    ScAddr,
    ScConstruction,
    ScLinkContent,
    ScTemplate,
    ScType,
    ScLinkContentType,
    ScEventType,
    ScEventParams,
} from 'ts-sc-client';
import { Redirect } from 'react-router';
import Cookie from 'universal-cookie';
import { encryptPassword, decryptPassword } from '@api/sc/password';
import { checkEmail, translateWord } from '@api/sc/checkUser';

export const Registration = () => {
    // Get Cookies
    const cookie = new Cookie();
    const cookieUserAddr = cookie.get('userAddr')
        ? new ScAddr(parseInt(String(cookie.get('userAddr'))))
        : new ScAddr(0);
    const cookiePassword = cookie.get('password');

    const [username, setUsername] = useState<string>('');
    const [pass, setPass] = useState<string>('');
    const [repeatPass, setRepeatPass] = useState<string>('');

    const [userAddr, setUserAddr] = useState<ScAddr>(new ScAddr(0));

    const [errorNotEqual, setErrorNotEqual] = useState<boolean>(false);
    const [errorPassLength, setErrorPassLength] = useState<boolean>(false);
    const [errorUsernameLength, setErrorUsernameLength] = useState<boolean>(false);
    const [errorUserNameInUse, setErrorUserNameInUse] = useState<boolean>(false);
    const [redirectError, setRedirectError] = useState<boolean>(false);

    const onRegisterResult = async (addr: ScAddr, edgeAddr: ScAddr, edgeToResultAddr: ScAddr) => {
        const baseKeynodes = [{ id: 'nrel_password', type: ScType.NodeConstNoRole }];
        const keynodes = await client.resolveKeynodes(baseKeynodes);

        let template = new ScTemplate();
        template.triple(ScType.NodeVar, edgeToResultAddr, [ScType.NodeVar, '_result']);
        template.triple('_result', ScType.EdgeAccessVarPosPerm, [ScType.NodeVar, '_user']);
        template.tripleWithRelation(
            '_user',
            ScType.EdgeDCommonVar,
            [ScType.LinkVar, '_password'],
            ScType.EdgeAccessVarPosPerm,
            keynodes['nrel_password'],
        );
        const result = await client.templateSearch(template);
        if (result.length > 0) {
            cookie.set('userAddr', result[0].get('_user').value);
            cookie.set('password', (await client.getLinkContents([result[0].get('_password')]))[0].data);
            setUserAddr(result[0].get('_user'));
        }
    };

    useEffect(() => {
        (async () => {
            if (cookieUserAddr.isValid() && cookiePassword) {
                setRedirectError(true);
            }
            var userLang = navigator.language;
            console.log(userLang);

            const baseKeynodes = [{ id: 'nrel_answer', type: ScType.NodeConstNoRole }];
            const keynodes = await client.resolveKeynodes(baseKeynodes);

            const eventParams = new ScEventParams(
                keynodes['nrel_answer'],
                ScEventType.AddOutgoingEdge,
                onRegisterResult,
            );
            await client.eventsCreate([eventParams]);
        })();
    }, []);

    const registerUser = async (name: string, password: string) => {
        const baseKeynodes = [
            { id: 'concept_users', type: ScType.NodeConstClass },
            { id: 'nrel_email', type: ScType.NodeConstNoRole },
        ];
        const keynodes = await client.resolveKeynodes(baseKeynodes);
        let userFound = false;
        const [linksArray] = await client.getLinksByContents([encryptPassword(name)]);
        console.log(linksArray);

        for (let i = 0; i < linksArray.length; i++) {
            let template = new ScTemplate();
            template.triple(keynodes['concept_users'], ScType.EdgeAccessVarPosPerm, [ScType.NodeVar, '_user']);
            template.tripleWithRelation(
                '_user',
                ScType.EdgeDCommonVar,
                linksArray[i],
                ScType.EdgeAccessVarPosPerm,
                keynodes['nrel_email'],
            );
            const result = await client.templateSearch(template);
            if (result.length > 0) userFound = true;
        }

        if (!userFound) {
            name = encryptPassword(name);
            password = encryptPassword(password);

            const baseKeynodes = [
                { id: 'action_register_user', type: ScType.NodeConstClass },
                { id: 'question_initiated', type: ScType.NodeConstClass },
                { id: 'question', type: ScType.NodeConstClass },
            ];

            const helpKeynodes = [
                { id: 'rrel_1', type: ScType.NodeConstRole },
                { id: 'rrel_2', type: ScType.NodeConstRole },
            ];

            const keynodes = await client.resolveKeynodes(baseKeynodes);
            const hKeynodes = await client.resolveKeynodes(helpKeynodes);

            const construction = new ScConstruction();
            construction.createNode(ScType.NodeConst, 'action_node');
            construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['action_register_user'], 'action_node');
            construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['question_initiated'], 'action_node');
            construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['question'], 'action_node');
            construction.createLink(ScType.LinkConst, new ScLinkContent(name, ScLinkContentType.String), 'username');
            construction.createLink(
                ScType.LinkConst,
                new ScLinkContent(password, ScLinkContentType.String),
                'password',
            );
            construction.createEdge(ScType.EdgeAccessConstPosPerm, 'action_node', 'username', 'name_edge');
            construction.createEdge(ScType.EdgeAccessConstPosPerm, 'action_node', 'password', 'pass_edge');
            construction.createEdge(ScType.EdgeAccessConstPosPerm, hKeynodes['rrel_1'], 'name_edge');
            construction.createEdge(ScType.EdgeAccessConstPosPerm, hKeynodes['rrel_2'], 'pass_edge');

            await client.createElements(construction);
        } else {
            setErrorUserNameInUse(true);
        }
    };

    const check = async (e) => {
        e.preventDefault();

        setErrorNotEqual(false);
        setErrorPassLength(false);
        setErrorUserNameInUse(false);

        if (username.length < 3) {
            setErrorUsernameLength(true);
            return;
        } else if (pass !== repeatPass) {
            setErrorNotEqual(true);
            return;
        } else if (pass.length < 6) {
            setErrorPassLength(true);
            return;
        } else if (await checkEmail(username)) {
            setErrorUserNameInUse(true);
            return;
        } else {
            registerUser(username, pass);
        }
    };

    const updateChangePassword = (e: ChangeEvent<HTMLInputElement>) => {
        setPass(e.target.value);
    };

    const updateChangeRepeatPassword = (e: ChangeEvent<HTMLInputElement>) => {
        setRepeatPass(e.target.value);
    };

    const updateUsername = (e: ChangeEvent<HTMLInputElement>) => {
        setUsername(e.target.value);
    };

    return (
        <div>
            {userAddr.isValid() ? <Redirect to={{ pathname: routes.INTRO }} /> : ''}
            {redirectError ? <Redirect to={{ pathname: routes.HOME }} /> : ''}

            <Circle></Circle>
            <Circle1></Circle1>
            <Wrapper>
                <WrapperContent>
                    <Form>
                        <FormText>{translateWord('Регистрация', navigator.language)}</FormText>
                        <Input
                            type="string"
                            name="username"
                            placeholder={translateWord('Email', navigator.language)}
                            onChange={updateUsername}
                            required
                        ></Input>
                        {errorUsernameLength ? (
                            <Error>
                                {translateWord(
                                    'Имя пользователя должно состоять как минимум из 4 символов.',
                                    navigator.language,
                                )}
                            </Error>
                        ) : (
                            ''
                        )}
                        {errorUserNameInUse ? (
                            <Error>
                                {translateWord('Имя пользователя уже занято!', navigator.language)}
                                Используйте другое.
                            </Error>
                        ) : (
                            ''
                        )}
                        <Input
                            type="password"
                            name="pass1"
                            id="pass1"
                            placeholder={translateWord('Пароль', navigator.language)}
                            onChange={updateChangePassword}
                            required
                        ></Input>
                        {errorPassLength ? (
                            <Error>
                                {translateWord('Пароль должен состоять как минимум из 6 символов.', navigator.language)}
                            </Error>
                        ) : (
                            ''
                        )}
                        <Input
                            type="password"
                            name="pass2"
                            id="pass2"
                            placeholder={translateWord('Повторите пароль', navigator.language)}
                            onChange={updateChangeRepeatPassword}
                            required
                        ></Input>
                        {errorNotEqual ? (
                            <Error>{translateWord('Пароли не совпадают!', navigator.language)}</Error>
                        ) : (
                            ''
                        )}
                        <FormBtn
                            type="submit"
                            id="submit"
                            onClick={(e) => {
                                check(e);
                            }}
                        >
                            {translateWord('Создать', navigator.language)}
                        </FormBtn>
                        <Link href={routes.LOGIN}>{translateWord('Уже есть аккаунт?', navigator.language)}</Link>
                    </Form>
                </WrapperContent>
            </Wrapper>
        </div>
    );
};
