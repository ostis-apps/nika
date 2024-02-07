import React, { useEffect, useState, useReducer, ChangeEvent } from 'react';
import { Wrapper, Container, WrapperLinks, DeleteButton, WrLink, NavLink, Arrow, Linktitle } from './styled';
import { client } from '@api';
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { ScTemplate, ScType, ScEventType } from 'ts-sc-client';
import { Redirect } from 'react-router';
import { checkUser, getFontSizeFromSettings, getUserName, getUserSettings } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import styled from 'styled-components';

export const Saved = () => {
    // Get Cookies
    const cookie = new Cookie();
    const cookieUserAddr = cookie.get('userAddr')
        ? new ScAddr(parseInt(String(cookie.get('userAddr'))))
        : new ScAddr(0);
    const cookiePassword = cookie.get('password');

    const [redirectError, setRedirectError] = useState<boolean | undefined>(undefined);
    const [noDesireError, setNoDesireError] = useState<boolean | undefined>(undefined);
    const [userName, setUserName] = useState<string | undefined>(undefined);

    const [links, setLinks] = useState<String[]>([]);
    const [linkNames, setLinkNames] = useState<String[]>([]);
    const [userTheme, setUserTheme] = useState<String>('dark');

    const [accentColor, setAccentColor] = useState<string | undefined>('black');
    const [addrsMap, setAddrsMaps] = useState<ScAddr[]>([]);
    const [params, setParams] = useState<{}>({});

    const [update, setUpdate] = useState<number>(0);

    const check = async () => {
        if (cookieUserAddr.isValid() && cookiePassword) {
            const name = await getUserName(cookieUserAddr);
            if (!(await checkUser(cookieUserAddr, cookiePassword))) {
                setRedirectError(true);
                return;
            } else if (!name) {
                setNoDesireError(true);
                return;
            } else {
                setUserName(name);
            }
        } else setRedirectError(true);
    };

    useEffect(() => {
        (async () => {
            check();

            const userSettings = await getUserSettings(cookieUserAddr);
            setParams(userSettings);
            setAccentColor(userSettings['nrel_accent_color']);
            setUserTheme(userSettings['nrel_theme']);

            const template = new ScTemplate();
            const baseKeynodes = [
                { id: 'nrel_saved', type: ScType.NodeConstNoRole },
                { id: 'nrel_main_idtf', type: ScType.NodeConstNoRole },
                { id: 'nrel_link', type: ScType.NodeConstNoRole },
            ];
            const keynodes = await client.resolveKeynodes(baseKeynodes);
            let varNameLinks: String[] = [],
                varLinks: String[] = [];

            template.tripleWithRelation(
                cookieUserAddr,
                ScType.EdgeDCommonVar,
                [ScType.NodeVar, '_saved'],
                ScType.EdgeAccessVarPosPerm,
                keynodes['nrel_saved'],
            );

            template.triple('_saved', ScType.EdgeDCommonVar, [ScType.NodeVar, '_saved_map']);

            template.tripleWithRelation(
                '_saved_map',
                ScType.EdgeDCommonVar,
                [ScType.LinkVar, '_link_name'],
                ScType.EdgeAccessVarPosPerm,
                keynodes['nrel_main_idtf'],
            );

            template.tripleWithRelation(
                '_saved_map',
                ScType.EdgeDCommonVar,
                [ScType.LinkVar, '_link_href'],
                ScType.EdgeAccessVarPosPerm,
                keynodes['nrel_link'],
            );
            const result = await client.templateSearch(template);
            let mapAddrs: ScAddr[] = [];

            if (result.length > 0) {
                for (let i = 0; i < result.length; i++) {
                    const element = result[i];
                    mapAddrs.push(element.get('_saved_map'));
                    const href = String((await client.getLinkContents([element.get('_link_href')]))[0].data);
                    const name = String((await client.getLinkContents([element.get('_link_name')]))[0].data);
                    varNameLinks.push(name);
                    varLinks.push(href);
                }
            }

            setLinkNames(varNameLinks);
            setLinks(varLinks);
            setAddrsMaps(mapAddrs);
        })();
    }, []);

    const deleteMap = async (event, key) => {
        await client.deleteElements([addrsMap[key]]);
        let allLinks = links;
        let allLinkNames = linkNames;
        allLinks.splice(key, 1);
        allLinkNames.splice(key, 1);

        setLinkNames(allLinkNames);
        setLinks(allLinks);
        setUpdate(update + 1);
    };
    useEffect(() => {
        console.log('delete');
    }, [update]);

    return (
        <>
            <NavLink href={routes.HOME} style={{ background: '#413d3d' }} className="nav">
                <Arrow></Arrow>
                <Linktitle className="title">Назад</Linktitle>
            </NavLink>
            <Wrapper>
                <Container>
                    <h2
                        style={{
                            color: 'white',
                            fontWeight: '900',
                            fontSize: getFontSizeFromSettings(params['nrel_font_size']),
                        }}
                    >
                        Сохраненное
                    </h2>
                    <WrapperLinks className="wrapper_saved_links">
                        {linkNames.map((item, index) => {
                            const Link = styled.a`
                                flex-basis: 100%;
                                padding: 10px;
                                border-radius: 10px;
                                color: white;
                                font-weight: 900;
                                background: rgba(256, 256, 256, 0.2);
                                transition: all 0.5s ease;
                                display: flex;
                                justify-content: space-between;
                                align-items: center;
                                :hover {
                                    color: ${accentColor};
                                    opacity: 0.6;
                                }
                            `;

                            return (
                                <WrLink>
                                    <Link
                                        href={links[index] as string}
                                        style={{ fontSize: getFontSizeFromSettings(params['nrel_font_size']) }}
                                    >
                                        {item}
                                    </Link>
                                    <DeleteButton key={index} onClick={(event) => deleteMap(event, index)}>
                                        <svg
                                            xmlns="http://www.w3.org/2000/svg"
                                            width="20"
                                            height="20"
                                            fill="currentColor"
                                            viewBox="0 0 16 16"
                                        >
                                            <path d="M5.5 5.5A.5.5 0 0 1 6 6v6a.5.5 0 0 1-1 0V6a.5.5 0 0 1 .5-.5m2.5 0a.5.5 0 0 1 .5.5v6a.5.5 0 0 1-1 0V6a.5.5 0 0 1 .5-.5m3 .5a.5.5 0 0 0-1 0v6a.5.5 0 0 0 1 0z" />
                                            <path d="M14.5 3a1 1 0 0 1-1 1H13v9a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2V4h-.5a1 1 0 0 1-1-1V2a1 1 0 0 1 1-1H6a1 1 0 0 1 1-1h2a1 1 0 0 1 1 1h3.5a1 1 0 0 1 1 1zM4.118 4 4 4.059V13a1 1 0 0 0 1 1h6a1 1 0 0 0 1-1V4.059L11.882 4zM2.5 3h11V2h-11z" />
                                        </svg>
                                    </DeleteButton>
                                </WrLink>
                            );
                        })}
                    </WrapperLinks>
                </Container>
            </Wrapper>
        </>
    );
};
