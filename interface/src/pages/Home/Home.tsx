import React, { useEffect, useState, useReducer, ChangeEvent } from 'react';
import {
    WrapperCircle,
    WrapperInf,
    ContainerInf,
    WrapperBtns,
    WrapperHead,
    WrapperWidget,
    ContentHead,
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
import { checkUser, getUserName, getUserSettings, getFontSizeFromSettings } from '@api/sc/checkUser';
import { ReactComponent as LangIcon } from '@assets/icon/lang.svg';
import { ReactComponent as SavedIcon } from '@assets/icon/saved.svg';
import Cookie from 'universal-cookie';
import styled from 'styled-components';
const weather_api_key = '1c9304a164f14d788f964943232410';

export const Home = () => {
    type w = {
        day: string;
        temp: string;
        icon: string;
    };

    // Get Cookies
    const cookie = new Cookie();
    const cookieUserAddr = cookie.get('userAddr')
        ? new ScAddr(parseInt(String(cookie.get('userAddr'))))
        : new ScAddr(0);
    const cookiePassword = cookie.get('password');

    const [weather, setWeather] = useState<w[]>([]);

    const [redirectError, setRedirectError] = useState<boolean | undefined>(undefined);
    const [noDesireError, setNoDesireError] = useState<boolean | undefined>(undefined);
    const [userName, setUserName] = useState<string | undefined>(undefined);
    const [city, setCity] = useState<string>('');

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

    const getApi = async (url: string) => {
        return (await fetch(url)).json();
    };

    useEffect(() => {
        (async () => {
            // Get Weathe
            check();

            setParams(await getUserSettings(cookieUserAddr));
            console.log(params);
        })();
    }, []);

    useEffect(() => {
        (async () => {
            setCity((await getApi('http://ip-api.com/json/'))['city']);
        })();
    });

    useEffect(() => {
        (async () => {
            let Day: any = new Date().getDay();
            let Month: any = new Date().getMonth();
            let Year: any = new Date().getFullYear();
            console.log(Day, Month, Year);
        })();
    }, [city]);

    const logoutUser = (e) => {
        e.preventDefault();
        cookie.remove('userAddr');
        cookie.remove('pass');
        setRedirectError(true);
    };

    const LangBtn = styled.a`
        cursor: pointer;
        background: none;
        border: none;
        transition: all 0.5s ease;
        :hover {
            opacity: 0.5;
        }
        svg path {
            fill: white;
            fill: ${params['nrel_theme'] == 'dark' ? 'white' : params['nrel_accent_color']};
        }
    `;
    console.log(params['nrel_accent_color']);
    return (
        <div>
            {redirectError ? <Redirect to={{ pathname: routes.LOGIN }} /> : ''}
            {noDesireError ? <Redirect to={{ pathname: routes.INTRO }} /> : ''}

            <div style={params['nrel_theme'] == 'light' ? { background: params['nrel_accent_color'] } : {}}>
                <WrapperCircle style={{ background: params['nrel_accent_color'] }}></WrapperCircle>

                <WrapperInf>
                    <WrapperHead style={params['nrel_theme'] == 'light' ? { border: '1ps solid white' } : {}}>
                        <ContainerInf>
                            <ContentHead>
                                <UserName
                                    style={{ fontSize: getFontSizeFromSettings(params['nrel_font_size']) }}
                                    onClick={(e) => logoutUser(e)}
                                >
                                    {userName}
                                </UserName>
                                <LangBtn href={routes.SETTINGS}>
                                    <LangIcon style={langStyles} />
                                </LangBtn>
                            </ContentHead>
                        </ContainerInf>
                    </WrapperHead>

                    <BtnsContainer>
                        <BtnChat
                            style={{ fontSize: getFontSizeFromSettings(params['nrel_font_size']) }}
                            href={routes.CHAT}
                        >
                            Чат
                        </BtnChat>
                        <WrapperBtns>
                            <BtnGames
                                style={{ fontSize: getFontSizeFromSettings(params['nrel_font_size']) }}
                                href={routes.CHAT}
                            >
                                Развлечения
                            </BtnGames>
                            <BtnSaved href={routes.SAVED}>
                                <WrapperSaved>
                                    <SavedIcon style={savedStyles} />
                                </WrapperSaved>
                            </BtnSaved>
                        </WrapperBtns>
                    </BtnsContainer>

                    <WrapperWidget>
                        <WidgetWeather>Weather</WidgetWeather>
                        <WidgetMap href={routes.OWNMAP}></WidgetMap>
                    </WrapperWidget>
                </WrapperInf>
            </div>
        </div>
    );
};
