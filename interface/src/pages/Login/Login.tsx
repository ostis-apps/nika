import React, { useEffect, useState, useReducer, ChangeEvent } from 'react';
import { Circle, Circle1, Wrapper, WrapperContent, Form, FormText, Input, FormBtn, Link, Error } from './styled';
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
import { checkEmail, translateWord } from '@api/sc/checkUser';
import { encryptPassword, decryptPassword } from '@api/sc/password';

export const Login = () => {
    // Get Cookies
    const cookie = new Cookie();
    const cookieUserAddr = cookie.get('userAddr')
        ? new ScAddr(parseInt(String(cookie.get('userAddr'))))
        : new ScAddr(0);
    const cookiePassword = cookie.get('password');

    const [userAuthorised, setUserAuthorised] = useState<boolean>(false);
    const [redirectError, setRedirectError] = useState<boolean>(false);
    const [pass, setPass] = useState<string>('');
    const [username, setUsername] = useState<string>('');
    const [errorLogin, setErrorLogin] = useState<boolean>(false);
    const [errorEmpty, setErrorEmpty] = useState<boolean>(false);

    const onAuthorisedResult = async (addr: ScAddr, edgeAddr: ScAddr, edgeToResultAddr: ScAddr) => {
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
            setUserAuthorised(true);
        } else {
            console.log('Error with authorise');
            setErrorLogin(true);
        }
    };
    const [, forceUpdate] = useReducer((x) => x + 1, 0);

    useEffect(() => {
        (async () => {
            forceUpdate();
        })();
    }, [userAuthorised]);

    useEffect(() => {
        (async () => {
            if (cookieUserAddr.isValid() && cookiePassword) {
                setRedirectError(true);
            }

            const baseKeynodes = [{ id: 'nrel_answer', type: ScType.NodeConstNoRole }];
            const keynodes = await client.resolveKeynodes(baseKeynodes);

            const eventParams = new ScEventParams(
                keynodes['nrel_answer'],
                ScEventType.AddOutgoingEdge,
                onAuthorisedResult,
            );
            await client.eventsCreate([eventParams]);
        })();
    }, []);

    const loginUser = async (name: string, password: string) => {
        const baseKeynodes = [
            { id: 'action_authorise_user', type: ScType.NodeConstClass },
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
        construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['action_authorise_user'], 'action_node');
        construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['question_initiated'], 'action_node');
        construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['question'], 'action_node');
        construction.createLink(
            ScType.LinkConst,
            new ScLinkContent(encryptPassword(name), ScLinkContentType.String),
            'username',
        );
        construction.createLink(
            ScType.LinkConst,
            new ScLinkContent(encryptPassword(password), ScLinkContentType.String),
            'password',
        );
        construction.createEdge(ScType.EdgeAccessConstPosPerm, 'action_node', 'username', 'name_edge');
        construction.createEdge(ScType.EdgeAccessConstPosPerm, 'action_node', 'password', 'pass_edge');
        construction.createEdge(ScType.EdgeAccessConstPosPerm, hKeynodes['rrel_1'], 'name_edge');
        construction.createEdge(ScType.EdgeAccessConstPosPerm, hKeynodes['rrel_2'], 'pass_edge');

        await client.createElements(construction);
    };

    const updateUsername = (e: ChangeEvent<HTMLInputElement>) => {
        setUsername(e.target.value);
    };

    const updatePass = (e: ChangeEvent<HTMLInputElement>) => {
        setPass(e.target.value);
    };

    const check = (e) => {
        e.preventDefault();

        setErrorEmpty(false);
        setErrorLogin(false);

        if (username == '' || pass == '') {
            setErrorEmpty(true);
            return 1;
        }

        loginUser(username, pass);
    };

    return (
        <div>
            {userAuthorised ? <Redirect to={{ pathname: routes.HOME }} /> : ''}
            {redirectError ? <Redirect to={{ pathname: routes.HOME }} /> : ''}

            <Circle></Circle>
            <Circle1></Circle1>
            <Wrapper>
                <WrapperContent>
                    <Form>
                        <FormText>{translateWord('Войти', navigator.language)}</FormText>
                        <Input
                            type="text"
                            name="username"
                            placeholder={translateWord('Email', navigator.language)}
                            onChange={updateUsername}
                            required
                        ></Input>
                        <Input
                            type="password"
                            name="pass"
                            id="pass"
                            placeholder={translateWord('Пароль', navigator.language)}
                            onChange={updatePass}
                            required
                        ></Input>
                        {errorEmpty ? (
                            <Error>{translateWord('Поля должны быть заполнены!', navigator.language)}</Error>
                        ) : (
                            ''
                        )}
                        {errorLogin ? (
                            <Error>
                                {translateWord('Неверное имя пользоватлеься или пароль!', navigator.language)}
                            </Error>
                        ) : (
                            ''
                        )}
                        <FormBtn type="submit" id="submit" onClick={check}>
                            {translateWord('Войти', navigator.language)}
                        </FormBtn>
                        <Link href={routes.REGISTRATION}>{translateWord('еще нет аккаунта?', navigator.language)}</Link>
                    </Form>
                </WrapperContent>
            </Wrapper>
        </div>
    );
};
