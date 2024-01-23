import React, { useEffect, useState, useReducer, ChangeEvent } from 'react';
import {
    WrapperCircle,
    WrapperInf,
    ContainerInf,
    WrapperBtns,
    WrapperHead,
    WrapperWidget,
    ContentHead,
    LangBtn,
    UserName,
    BtnChat,
    BtnSaved,
    BtnGames,
} from './styled';
import { client } from '@api';
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { ScTemplate, ScType, ScEventType } from 'ts-sc-client';
import { Redirect } from 'react-router';
import { checkUser, getUserName } from '@api/sc/checkUser';
import { setCookie, getCookie, removeCookie } from 'typescript-cookie';
import { ReactComponent as LangIcon } from '@assets/icon/lang.svg';

export const Home = () => {
    const [redirectError, setRedirectError] = useState<boolean | undefined>(undefined);
    const [noDesireError, setNoDesireError] = useState<boolean | undefined>(undefined);
    const [userName, setUserName] = useState<string | undefined>(undefined);
    const userAddr = getCookie('userAddr') ? new ScAddr(parseInt(String(getCookie('userAddr')))) : new ScAddr(0);
    const password = getCookie('password');

    const check = async () => {
        if (userAddr.isValid() && password) {
            const name = await getUserName(userAddr);
            if (!(await checkUser(userAddr, password))) {
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
        })();
    }, []);

    const logoutUser = (e) => {
        e.preventDefault();
        removeCookie('userAddr', { path: '' });
        removeCookie('pass', { path: '' });
        setRedirectError(true);
    };

    const langStyles = {
        width: '30px',
        height: '30px',
    };

    return (
        <div>
            {redirectError ? <Redirect to={{ pathname: routes.LOGIN }} /> : ''}
            {noDesireError ? <Redirect to={{ pathname: routes.INTRO }} /> : ''}

            <div>
                <WrapperCircle></WrapperCircle>

                <WrapperInf>
                    <WrapperHead>
                        <ContainerInf>
                            <ContentHead>
                                <UserName href="#">{userName}</UserName>
                                <LangBtn>
                                    <LangIcon style={langStyles} />
                                </LangBtn>
                            </ContentHead>
                        </ContainerInf>
                    </WrapperHead>

                    <WrapperBtns>
                        <BtnChat></BtnChat>
                        <BtnSaved></BtnSaved>
                        <BtnGames></BtnGames>
                    </WrapperBtns>

                    <WrapperWidget></WrapperWidget>
                </WrapperInf>
            </div>
            <button onClick={logoutUser}>Logout</button>
        </div>
    );
};
