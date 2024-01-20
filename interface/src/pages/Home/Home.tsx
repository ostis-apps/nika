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
} from './styled'
import { client } from "@api";
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { ScTemplate, ScType, ScEventType } from 'ts-sc-client';
import { Redirect } from 'react-router';
import { checkUser } from '@api/sc/checkUser';

export const Home = () => {
    const [redirectError, setRedirectError] = useState<boolean>(false);
    const [userName, setUserName] = useState<string | number>('');

    console.log(document.cookie)
    const cookieParams = JSON.parse( document.cookie );
    const userAddr = new ScAddr(parseInt(cookieParams.userAddr));
    const password = cookieParams.pass;

    const check = async () => {
        if (document.cookie == '') {
            setRedirectError(true);
        } else
            if (!(await checkUser(userAddr, password))) {
                setRedirectError(true);
            }
    }

    check();

    const logoutUser = (e) => {
        e.preventDefault();
        document.cookie = '';
        setRedirectError(true);
    }

    const getUserName = async () => {
        const baseKeynodes = [
            { id: "nrel_password", type: ScType.NodeConstNoRole },
        ];
        const keynodes = await client.resolveKeynodes(baseKeynodes);

        const template = new ScTemplate();
        template.tripleWithRelation(
            userAddr,
            ScType.EdgeDCommonVar,
            [ScType.LinkVar, '_userName'],
            ScType.EdgeAccessVarPosPerm,
            keynodes['nrel_name'],
        )
        const result = await client.templateSearch(template);
        if (result.length > 0) {
            setUserName((await client.getLinkContents([result[0].get('_userName')]))[0].data);
        }
    }     

    return (
        <div>
            {redirectError ? <Redirect to={{ pathname: routes.LOGIN }}/> : (
                <div>
                    <h2>{ userName }</h2>
                    <button onClick={logoutUser}>Logout</button>    
                </div>
            )}
        </div>
        
    );
};