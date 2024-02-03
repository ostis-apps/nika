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
    WidgetWeather,
    WidgetMap,
    langStyles,
    BtnsContainer,
    savedStyles,
    WrapperSaved,
} from './styled';
import { client } from '@api';
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { ScTemplate, ScType, ScEventType } from 'ts-sc-client';
import { Redirect } from 'react-router';
import { checkUser, getUserName, getUserSettings } from '@api/sc/checkUser';
import { ReactComponent as LangIcon } from '@assets/icon/lang.svg';
import { ReactComponent as SavedIcon } from '@assets/icon/saved.svg';
import Cookie from 'universal-cookie';

export const Home = () => {
    // Get Cookies
    const cookie = new Cookie();
    const cookieUserAddr = cookie.get('userAddr')
        ? new ScAddr(parseInt(String(cookie.get('userAddr'))))
        : new ScAddr(0);
    const cookiePassword = cookie.get('password');

    const [redirectError, setRedirectError] = useState<boolean | undefined>(undefined);
    const [noDesireError, setNoDesireError] = useState<boolean | undefined>(undefined);
    const [userName, setUserName] = useState<string | undefined>(undefined);

    const [params, setParams] = useState<{}>({});

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
            setParams(await getUserSettings(cookieUserAddr));
            console.log(params);
        })();
    }, []);

    const logoutUser = (e) => {
        e.preventDefault();
        cookie.remove('userAddr');
        cookie.remove('pass');
        setRedirectError(true);
    };

    return (
        <div>
            {redirectError ? <Redirect to={{ pathname: routes.LOGIN }} /> : ''}
            {noDesireError ? <Redirect to={{ pathname: routes.INTRO }} /> : ''}

            <div style={params['nrel_theme'] == 'light' ? { background: params['nrel_accent_color'] } : {}}>
                <WrapperCircle></WrapperCircle>

                <WrapperInf>
                    <WrapperHead style={params['nrel_theme'] == 'light' ? { border: '1ps solid white' } : {}}>
                        <ContainerInf>
                            <ContentHead>
                                <UserName onClick={(e) => logoutUser(e)}>{userName}</UserName>
                                <LangBtn href={routes.SETTINGS}>
                                    <LangIcon style={langStyles} />
                                </LangBtn>
                            </ContentHead>
                        </ContainerInf>
                    </WrapperHead>

                    <BtnsContainer>
                        <BtnChat href={routes.CHAT}>Чат</BtnChat>
                        <WrapperBtns>
                            <BtnGames href={routes.CHAT}>Развлечения</BtnGames>
                            <BtnSaved href={routes.SAVED}>
                                <WrapperSaved>
                                    <SavedIcon style={savedStyles} />
                                </WrapperSaved>
                            </BtnSaved>
                        </WrapperBtns>
                    </BtnsContainer>

                    <WrapperWidget>
                        <WidgetWeather>Weather</WidgetWeather>
                        <WidgetMap>Map</WidgetMap>
                    </WrapperWidget>
                </WrapperInf>
            </div>
        </div>
    );
};
