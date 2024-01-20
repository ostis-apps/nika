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
import { setCookie, getCookie, removeCookie } from "typescript-cookie";


export const Home = () => {
    const [redirectError, setRedirectError] = useState<boolean | undefined>(undefined);
    const [userName, setUserName] = useState<string | undefined>(undefined);
    const userAddr = getCookie('userAddr') ? new ScAddr(parseInt(String(getCookie('userAddr')))) : new ScAddr(0)
    const password = getCookie('pass')
  
    const check = async () => {
        if (userAddr.isValid() && password) {
            if (!(await checkUser(userAddr, password))) {
                setRedirectError(true);
            }
        } else
            setRedirectError(true);     
    }
    check();

    const logoutUser = (e) => {
        e.preventDefault(); 
        removeCookie('userAddr', {path: ''});
        removeCookie('pass', {path: ''});
        setRedirectError(true);
    }

    const getUserName = async () => {
        const baseKeynodes = [
            { id: "nrel_name", type: ScType.NodeConstNoRole },
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
        console.log(result.length)
        if (result.length > 0) {
            setUserName(String((await client.getLinkContents([result[0].get('_userName')]))[0].data));
        }
    }     
    getUserName()

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