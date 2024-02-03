import React, { useEffect, useState, useReducer, ChangeEvent } from 'react';
import { Wrapper, Container, WrapperLinks, Link } from './styled';
import { client } from '@api';
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { ScTemplate, ScType, ScEventType } from 'ts-sc-client';
import { Redirect } from 'react-router';
import { checkUser, getUserName } from '@api/sc/checkUser';
import { ReactComponent as LangIcon } from '@assets/icon/lang.svg';
import { ReactComponent as SavedIcon } from '@assets/icon/saved.svg';
import Cookie from 'universal-cookie';

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

            if (result.length > 0) {
                for (let i = 0; i < result.length; i++) {
                    const element = result[i];
                    const href = String((await client.getLinkContents([element.get('_link_href')]))[0].data);
                    const name = String((await client.getLinkContents([element.get('_link_name')]))[0].data);
                    varNameLinks.push(name);
                    varLinks.push(href);
                }
            }

            setLinkNames(varNameLinks);
            setLinks(varLinks);
        })();
    }, []);

    return (
        <>
            <Wrapper>
                <Container>
                    <h2>Сохраненное</h2>
                    <WrapperLinks>
                        {linkNames.map((item, index) => {
                            return <Link href={links[index] as string}>{item}</Link>;
                        })}
                    </WrapperLinks>
                </Container>
            </Wrapper>
        </>
    );
};
