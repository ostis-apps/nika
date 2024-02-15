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
import { checkUser, getUserName, getUserSettings, getFontSizeFromSettings, translateWord } from '@api/sc/checkUser';
import { ReactComponent as LangIcon } from '@assets/icon/lang.svg';
import { ReactComponent as SavedIcon } from '@assets/icon/saved.svg';
import Cookie from 'universal-cookie';
import styled from 'styled-components';
const weather_api_key = 'b2c6abeb0bfa8c28395919438f4eb3d1';

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
    const [temp, setTemp] = useState<string>('');
    const [icon, setIcon] = useState<string>('');

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
            // Get Weathe
            check();

            setParams(await getUserSettings(cookieUserAddr));

            await fetch('http://ip-api.com/json/')
                .then((result) => result.json())
                .then(async (result) => {
                    setCity(result['city']);
                    await fetch(
                        `https://api.openweathermap.org/data/2.5/weather?appid=${weather_api_key}&lat=${result['lat']}&lon=${result['lon']}`,
                    )
                        .then((res) => res.json())
                        .then((res) => {
                            setTemp(String(Math.round(parseFloat(res['main']['temp']) - 273.15)) as string);
                            const description = res['weather'][0]['main'];
                            if (description == 'Clear')
                                setIcon(
                                    '<div class="weather-image"><svg enable-background="new 0 0 32 32" version="1.1" viewBox="0 0 32 32" xml:space="preserve" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"><g id="Layer_2"/><g id="Layer_3"/><g id="Layer_4"/><g id="Layer_5"/><g id="Layer_6"/><g id="Layer_7"/><g id="Layer_8"/><g id="Layer_9"/><g id="Layer_10"/><g id="Layer_11"/><g id="Layer_12"/><g id="Layer_13"/><g id="Layer_14"/><g id="Layer_15"/><g id="Layer_16"/><g id="Layer_17"/><g id="Layer_18"/><g id="Layer_19"/><g id="Layer_20"/><g id="Layer_21"><g><path d="M26,16c0,5.5-4.5,10-10,10S6,21.5,6,16S10.5,6,16,6S26,10.5,26,16z" fill="#FFC10A"/></g><g><path d="M16,1c-0.6,0-1,0.4-1,1v2c0,0.6,0.4,1,1,1s1-0.4,1-1V2C17,1.4,16.6,1,16,1z" fill="#F44236"/><path d="M16,27c-0.6,0-1,0.4-1,1v2c0,0.6,0.4,1,1,1s1-0.4,1-1v-2C17,27.4,16.6,27,16,27z" fill="#F44236"/><path d="M30,15h-2c-0.6,0-1,0.4-1,1s0.4,1,1,1h2c0.6,0,1-0.4,1-1S30.6,15,30,15z" fill="#F44236"/><path d="M4,15H2c-0.6,0-1,0.4-1,1s0.4,1,1,1h2c0.6,0,1-0.4,1-1S4.6,15,4,15z" fill="#F44236"/><path d="M25.2,5.4l-1.4,1.4c-0.4,0.4-0.4,1,0,1.4c0.2,0.2,0.5,0.3,0.7,0.3s0.5-0.1,0.7-0.3l1.4-1.4    c0.4-0.4,0.4-1,0-1.4S25.6,5,25.2,5.4z" fill="#F44236"/><path d="M6.8,23.8l-1.4,1.4c-0.4,0.4-0.4,1,0,1.4c0.2,0.2,0.5,0.3,0.7,0.3s0.5-0.1,0.7-0.3l1.4-1.4    c0.4-0.4,0.4-1,0-1.4S7.2,23.4,6.8,23.8z" fill="#F44236"/><path d="M6.8,5.4C6.4,5,5.8,5,5.4,5.4s-0.4,1,0,1.4l1.4,1.4C7,8.4,7.3,8.5,7.5,8.5S8,8.4,8.2,8.2    c0.4-0.4,0.4-1,0-1.4L6.8,5.4z" fill="#F44236"/><path d="M25.2,23.8c-0.4-0.4-1-0.4-1.4,0s-0.4,1,0,1.4l1.4,1.4c0.2,0.2,0.5,0.3,0.7,0.3s0.5-0.1,0.7-0.3    c0.4-0.4,0.4-1,0-1.4L25.2,23.8z" fill="#F44236"/></g></g><g id="Layer_22"/><g id="Layer_23"/><g id="Layer_24"/><g id="Layer_25"/><g id="Wearher"/></svg></div>',
                                );
                            else if (description == 'Clouds')
                                setIcon(
                                    '<div class="weather-image"><svg enable-background="new 0 0 32 32" version="1.1" viewBox="0 0 32 32" xml:space="preserve" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"><g id="Layer_2"/><g id="Layer_3"/><g id="Layer_4"/><g id="Layer_5"/><g id="Layer_6"/><g id="Layer_7"/><g id="Layer_8"/><g id="Layer_9"/><g id="Layer_10"/><g id="Layer_11"/><g id="Layer_12"/><g id="Layer_13"/><g id="Layer_14"/><g id="Layer_15"/><g id="Layer_16"/><g id="Layer_17"/><g id="Layer_18"/><g id="Layer_19"/><g id="Layer_20"/><g id="Layer_21"/><g id="Layer_22"/><g id="Layer_23"/><g id="Layer_24"><g><path d="M16.1,9.5c-1.4,0.9-2.4,2.2-2.8,3.7c-0.1,0.3-0.4,0.6-0.7,0.7c-0.1,0-0.2,0-0.2,0c-0.3,0-0.5-0.1-0.7-0.3    C10.7,12.5,9.4,12,8,12c-1.2,0-2.3,0.4-3.2,1.1c-0.3,0.2-0.6,0.3-1,0.2c-0.3-0.1-0.6-0.4-0.7-0.7C3.1,12.1,3,11.5,3,11    c0-3.9,3.1-7,7-7c2.8,0,5.3,1.7,6.5,4.3C16.6,8.7,16.5,9.2,16.1,9.5z" fill="#FFC10A"/></g><g><path d="M31,15c0,5-4.5,9-10,9H8c-3.9,0-7-3.1-7-7c0-2.1,0.9-4,2.5-5.4C4.8,10.6,6.4,10,8,10    c1.4,0,2.8,0.4,3.9,1.2c0.7-1.3,1.7-2.5,3-3.4C16.7,6.6,18.8,6,21,6C26.5,6,31,10,31,15z" fill="#ffffff"/></g></g><g id="Layer_25"/><g id="Wearher"/></svg></div>',
                                );
                            else if (description == 'Rain')
                                setIcon(
                                    '<div class="weather-image"><svg enable-background="new 0 0 32 32" version="1.1" viewBox="0 0 32 32" xml:space="preserve" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"><g id="Layer_2"/><g id="Layer_3"/><g id="Layer_4"/><g id="Layer_5"/><g id="Layer_6"/><g id="Layer_7"/><g id="Layer_8"/><g id="Layer_9"/><g id="Layer_10"/><g id="Layer_11"/><g id="Layer_12"/><g id="Layer_13"/><g id="Layer_14"/><g id="Layer_15"/><g id="Layer_16"/><g id="Layer_17"/><g id="Layer_18"/><g id="Layer_19"/><g id="Layer_20"/><g id="Layer_21"/><g id="Layer_22"><g><path d="M16.5,22.1c-0.5-0.3-1.1-0.1-1.4,0.4l-1.2,2.2c-0.3,0.5-0.1,1.1,0.4,1.4c0.2,0.1,0.3,0.1,0.5,0.1    c0.3,0,0.7-0.2,0.9-0.5l1.2-2.2C17.1,23,17,22.4,16.5,22.1z" fill="#16BCD4"/><path d="M13.7,27c-0.5-0.3-1.1-0.1-1.4,0.4l-1.2,2.2c-0.3,0.5-0.1,1.1,0.4,1.4C11.7,31,11.8,31,12,31    c0.3,0,0.7-0.2,0.9-0.5l1.2-2.2C14.4,27.8,14.2,27.2,13.7,27z" fill="#16BCD4"/><path d="M10.5,22.1C10,21.9,9.4,22,9.1,22.5l-1.2,2.2c-0.3,0.5-0.1,1.1,0.4,1.4c0.2,0.1,0.3,0.1,0.5,0.1    c0.3,0,0.7-0.2,0.9-0.5l1.2-2.2C11.1,23,11,22.4,10.5,22.1z" fill="#16BCD4"/><path d="M7.7,27c-0.5-0.3-1.1-0.1-1.4,0.4l-1.2,2.2C4.9,30,5,30.6,5.5,30.9C5.7,31,5.8,31,6,31    c0.3,0,0.7-0.2,0.9-0.5l1.2-2.2C8.4,27.8,8.2,27.2,7.7,27z" fill="#16BCD4"/><path d="M22.5,22.1c-0.5-0.3-1.1-0.1-1.4,0.4l-1.2,2.2c-0.3,0.5-0.1,1.1,0.4,1.4c0.2,0.1,0.3,0.1,0.5,0.1    c0.3,0,0.7-0.2,0.9-0.5l1.2-2.2C23.1,23,23,22.4,22.5,22.1z" fill="#16BCD4"/><path d="M19.7,27c-0.5-0.3-1.1-0.1-1.4,0.4l-1.2,2.2c-0.3,0.5-0.1,1.1,0.4,1.4C17.7,31,17.8,31,18,31    c0.3,0,0.7-0.2,0.9-0.5l1.2-2.2C20.4,27.8,20.2,27.2,19.7,27z" fill="#16BCD4"/></g><g><path d="M31,15c0,5-4.5,9-10,9H8c-3.9,0-7-3.1-7-7s3.1-7,7-7c1.4,0,2.8,0.4,3.9,1.2C13.5,8.1,17.1,6,21,6    C26.5,6,31,10,31,15z" fill="#ffffff"/></g></g><g id="Layer_23"/><g id="Layer_24"/><g id="Layer_25"/><g id="Wearher"/></svg></div>',
                                );
                            else if (description == 'Drizzle' || description == 'Snow')
                                setIcon(
                                    '<div class="weather-image"><svg enable-background="new 0 0 32 32" version="1.1" viewBox="0 0 32 32" xml:space="preserve" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"><g id="Layer_2"/><g id="Layer_3"/><g id="Layer_4"/><g id="Layer_5"/><g id="Layer_6"/><g id="Layer_7"/><g id="Layer_8"/><g id="Layer_9"/><g id="Layer_10"/><g id="Layer_11"/><g id="Layer_12"/><g id="Layer_13"/><g id="Layer_14"/><g id="Layer_15"/><g id="Layer_16"/><g id="Layer_17"/><g id="Layer_18"/><g id="Layer_19"/><g id="Layer_20"><path d="M30,15H17V2c0-0.6-0.4-1-1-1s-1,0.4-1,1v13H2c-0.6,0-1,0.4-1,1s0.4,1,1,1h13v13c0,0.6,0.4,1,1,1s1-0.4,1-1   V17h13c0.6,0,1-0.4,1-1S30.6,15,30,15z" fill="#2197F3"/><g><path d="M16,11c-0.2,0-0.4-0.1-0.6-0.2l-5-4c-0.4-0.3-0.5-1-0.2-1.4c0.3-0.4,1-0.5,1.4-0.2L16,8.7l4.4-3.5    c0.4-0.3,1.1-0.3,1.4,0.2c0.3,0.4,0.3,1.1-0.2,1.4l-5,4C16.4,10.9,16.2,11,16,11z" fill="#16BCD4"/></g><g><path d="M21,27c-0.2,0-0.4-0.1-0.6-0.2L16,23.3l-4.4,3.5c-0.4,0.3-1.1,0.3-1.4-0.2c-0.3-0.4-0.3-1.1,0.2-1.4l5-4    c0.4-0.3,0.9-0.3,1.2,0l5,4c0.4,0.3,0.5,1,0.2,1.4C21.6,26.9,21.3,27,21,27z" fill="#16BCD4"/></g><g><path d="M6,22c-0.2,0-0.4-0.1-0.6-0.2c-0.4-0.3-0.5-1-0.2-1.4L8.7,16l-3.5-4.4c-0.3-0.4-0.3-1.1,0.2-1.4    c0.4-0.3,1.1-0.3,1.4,0.2l4,5c0.3,0.4,0.3,0.9,0,1.2l-4,5C6.6,21.9,6.3,22,6,22z" fill="#16BCD4"/></g><g><path d="M26,22c-0.3,0-0.6-0.1-0.8-0.4l-4-5c-0.3-0.4-0.3-0.9,0-1.2l4-5c0.3-0.4,1-0.5,1.4-0.2    c0.4,0.3,0.5,1,0.2,1.4L23.3,16l3.5,4.4c0.3,0.4,0.3,1.1-0.2,1.4C26.4,21.9,26.2,22,26,22z" fill="#16BCD4"/></g></g><g id="Layer_21"/><g id="Layer_22"/><g id="Layer_23"/><g id="Layer_24"/><g id="Layer_25"/><g id="Wearher"/></svg></div>',
                                );
                            else if (description == 'Thunderstorm')
                                setIcon(
                                    '<div class="weather-image"><svg enable-background="new 0 0 32 32" version="1.1" viewBox="0 0 32 32" xml:space="preserve" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"><g id="Layer_2"/><g id="Layer_3"/><g id="Layer_4"/><g id="Layer_5"/><g id="Layer_6"/><g id="Layer_7"/><g id="Layer_8"/><g id="Layer_9"/><g id="Layer_10"/><g id="Layer_11"/><g id="Layer_12"/><g id="Layer_13"/><g id="Layer_14"/><g id="Layer_15"/><g id="Layer_16"><g><path d="M31,15c0,4.4-3.5,8.1-8.3,8.9c0,0-0.1,0-0.1,0L19.7,24H8c-3.9,0-7-3.1-7-7s3.1-7,7-7    c1.4,0,2.8,0.4,3.9,1.2C13.5,8.1,17.1,6,21,6C26.5,6,31,10,31,15z" fill="#ffffff"/></g><g><path d="M26.9,16.9l-7.2,13.6c-0.2,0.3-0.5,0.5-0.9,0.5c-0.1,0-0.2,0-0.3,0c-0.5-0.1-0.8-0.6-0.7-1.1l1.1-8.9H14    c-0.4,0-0.7-0.2-0.9-0.5c-0.2-0.3-0.2-0.7,0-1l6-10c0.2-0.4,0.7-0.6,1.1-0.5c0.4,0.1,0.7,0.5,0.7,1v5.4h5c0.3,0,0.7,0.2,0.9,0.5    S27,16.6,26.9,16.9z" fill="#FFC10A"/></g></g><g id="Layer_17"/><g id="Layer_18"/><g id="Layer_19"/><g id="Layer_20"/><g id="Layer_21"/><g id="Layer_22"/><g id="Layer_23"/><g id="Layer_24"/><g id="Layer_25"/><g id="Wearher"/></svg></div>',
                                );
                            else
                                setIcon(
                                    '<div class="weather-image"><svg enable-background="new 0 0 32 32" version="1.1" viewBox="0 0 32 32" xml:space="preserve" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"><g id="Layer_2"/><g id="Layer_3"/><g id="Layer_4"/><g id="Layer_5"/><g id="Layer_6"/><g id="Layer_7"/><g id="Layer_8"/><g id="Layer_9"/><g id="Layer_10"/><g id="Layer_11"/><g id="Layer_12"><g><path d="M24,13H8c-0.6,0-1,0.4-1,1s0.4,1,1,1h16c0.6,0,1-0.4,1-1S24.6,13,24,13z" fill="#2197F3"/><path d="M19,18c0-0.6-0.4-1-1-1H6c-0.6,0-1,0.4-1,1s0.4,1,1,1h12C18.6,19,19,18.6,19,18z" fill="#2197F3"/><path d="M21,21H9c-0.6,0-1,0.4-1,1s0.4,1,1,1h12c0.6,0,1-0.4,1-1S21.6,21,21,21z" fill="#2197F3"/><path d="M20,25h-8c-0.6,0-1,0.4-1,1s0.4,1,1,1h8c0.6,0,1-0.4,1-1S20.6,25,20,25z" fill="#2197F3"/><path d="M18,29h-4c-0.6,0-1,0.4-1,1s0.4,1,1,1h4c0.6,0,1-0.4,1-1S18.6,29,18,29z" fill="#2197F3"/><path d="M28,1H2C1.4,1,1,1.4,1,2s0.4,1,1,1h26c0.6,0,1,0.4,1,1s-0.4,1-1,1H6C4.3,5,3,6.3,3,8s1.3,3,3,3h20    c0.6,0,1-0.4,1-1s-0.4-1-1-1H6C5.4,9,5,8.6,5,8s0.4-1,1-1h22c1.7,0,3-1.3,3-3S29.7,1,28,1z" fill="#2197F3"/></g></g><g id="Layer_13"/><g id="Layer_14"/><g id="Layer_15"/><g id="Layer_16"/><g id="Layer_17"/><g id="Layer_18"/><g id="Layer_19"/><g id="Layer_20"/><g id="Layer_21"/><g id="Layer_22"/><g id="Layer_23"/><g id="Layer_24"/><g id="Layer_25"/><g id="Wearher"/></svg></div>',
                                );
                        });
                });
        })();
    }, []);

    useEffect(() => {
        (async () => {})();
    }, []);

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
                            {translateWord('Чат', params['nrel_lang'])}
                        </BtnChat>
                        <WrapperBtns>
                            <BtnGames
                                style={{ fontSize: getFontSizeFromSettings(params['nrel_font_size']) }}
                                href={routes.CHAT}
                            >
                                {translateWord('Развлечения', params['nrel_lang'])}
                            </BtnGames>
                            <BtnSaved href={routes.SAVED}>
                                <WrapperSaved>
                                    <SavedIcon style={savedStyles} />
                                </WrapperSaved>
                            </BtnSaved>
                        </WrapperBtns>
                    </BtnsContainer>

                    <WrapperWidget>
                        <WidgetWeather>
                            <p style={{ fontSize: getFontSizeFromSettings(params['nrel_font_size']) }}>{city}</p>
                            <div dangerouslySetInnerHTML={{ __html: icon }} />
                            <p style={{ fontSize: getFontSizeFromSettings(params['nrel_font_size']) }}>{temp}</p>
                        </WidgetWeather>
                        <WidgetMap href={routes.OWNMAP}></WidgetMap>
                    </WrapperWidget>
                </WrapperInf>
            </div>
        </div>
    );
};
