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
import { checkUser, getUserName } from '@api/sc/checkUser';
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
                console.log(name);
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
        cookie.remove('userAddr');
        cookie.remove('pass');
        setRedirectError(true);
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
                            <BtnSaved href={routes.CHAT}>
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
