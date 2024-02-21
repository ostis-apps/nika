import React, { useEffect, useState, useReducer, ChangeEvent } from 'react';
import { WrapperSettings, Container, Setting, SettingsText, SaveSettings } from './styled';
import { client } from '@api';
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { ScTemplate, ScType } from 'ts-sc-client';
import { Redirect } from 'react-router';
import { checkUser } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import { translateWord, updateSettings, getUserSettings, getFontSizeFromSettings } from '@api/sc/checkUser';
import styled from 'styled-components';
import { ChatPageWrapper } from '@components/ChatPageWrapper';

export const Settings = () => {
    // Get Cookies
    const cookie = new Cookie();
    const userAddr = cookie.get('userAddr') ? new ScAddr(parseInt(String(cookie.get('userAddr')))) : new ScAddr(0);
    const password = cookie.get('password');

    const [settingTheme, setSettingTheme] = useState<string>('');
    const [settingFontSize, setSettingFontSize] = useState<string>('');
    const [settingAccentColor, setSettingAccentColor] = useState<string>('');
    const [settingLang, setSettingLang] = useState<string>('');
    const [settingInvalideMode, setSettingInvalideMode] = useState<string>('');

    const [settings, setSettings] = useState<Object>({});

    const [savedSettings, setSavedSettings] = useState<boolean>(false);
    const [redirectError, setRedirectError] = useState<boolean>(false);

    const [params, setParams] = useState<{}>({});

    const logoutUser = () => {
        cookie.remove('userAddr');
        cookie.remove('pass');
    };

    const check = async () => {
        if (userAddr.isValid() && password) {
            if (!(await checkUser(userAddr, password))) {
                logoutUser();
                setRedirectError(true);
            }
        } else {
            setRedirectError(true);
        }
    };

    useEffect(() => {
        (async () => {
            check();
            setSettings(await getUserSettings(userAddr));
        })();
    }, []);

    useEffect(() => {
        setSettingTheme(settings['nrel_theme']);
        setSettingFontSize(settings['nrel_font_size']);
        setSettingAccentColor(settings['nrel_accent_color']);
        setSettingLang(settings['nrel_lang']);
        setSettingInvalideMode(settings['nrel_invalid']);
    }, [settings]);

    const changeTheme = (e: ChangeEvent<HTMLSelectElement>) => {
        setSettingTheme(e.target.value);
    };

    const changeFontSize = (e: ChangeEvent<HTMLSelectElement>) => {
        setSettingFontSize(e.target.value);
    };

    const changeAccentColor = (e: ChangeEvent<HTMLInputElement>) => {
        setSettingAccentColor(e.target.value);
    };

    const changeLang = (e: ChangeEvent<HTMLSelectElement>) => {
        setSettingLang(e.target.value);
    };

    const changeInvalideMode = (e: ChangeEvent<HTMLInputElement>) => {
        setSettingInvalideMode(e.target.checked ? 'on' : 'off');
    };

    const saveSettings = async () => {
        console.log(settings);
        await updateSettings(userAddr, 'nrel_theme', settingTheme);
        console.log(settingFontSize);
        await updateSettings(userAddr, 'nrel_font_size', settingFontSize);
        await updateSettings(userAddr, 'nrel_accent_color', settingAccentColor);
        await updateSettings(userAddr, 'nrel_lang', settingLang);
        await updateSettings(userAddr, 'nrel_invalid', settingInvalideMode);
        setSettings(await getUserSettings(userAddr));
    };

    const Main = styled.div`
        display: flex;
        width: 100%;
        height: 100%;
        transition: all 0.5s ease;
    `;

    const Select = styled.select`
        option {
            background: rgb(51, 47, 47);
        }
    `;

    const NavLink = styled.a`
        display: flex;
        position: absolute;
        text-decoration: none;
        z-index: 1;
        top: 10px;
        left: 10px;
        gap: 12px;
        border-radius: 10px;
        align-items: center;
        height: 40px;
        color: white;
        margin-right: 10px;
        background: #413d3d;
        -webkit-box-shadow: 0px 0px 46px 21px rgba(34, 60, 80, 0.2);
        -moz-box-shadow: 0px 0px 46px 21px rgba(34, 60, 80, 0.2);
        box-shadow: 0px 0px 46px 21px rgba(34, 60, 80, 0.2);
        transition: all 0.5s ease-out;
        :hover {
            transform: translate(-1%, 0);
    `;

    const Arrow = styled.div`
        border: solid rgb(255, 255, 255);
        border-width: 0 3px 3px 0;
        display: inline-block;
        padding: 3px;
        width: 5px;
        height: 5px;
        margin-right: 5px;
        transform: translate(100%, 0) rotate(135deg);
        -webkit-transform: translate(100%, 0) rotate(135deg);
        cursor: pointer;
    `;

    const Linktitle = styled.div`
        color: rgb(255, 255, 255);
        font-weight: 700;
        padding: 0 10px 0 0;
    `;

    return (
        <>
            <NavLink href={routes.HOME} className="nav">
                <Arrow></Arrow>
                <Linktitle className="title">{translateWord('Назад', settings['nrel_lang'])}</Linktitle>
            </NavLink>
            <Main style={settingTheme == 'light' ? { background: settingAccentColor } : {}}>
                {savedSettings ? <Redirect to={{ pathname: routes.HOME }} /> : ''}
                {redirectError ? <Redirect to={{ pathname: routes.LOGIN }} /> : ''}

                <Container>
                    <SettingsText style={{ fontSize: getFontSizeFromSettings(settingFontSize) }}>
                        {translateWord('Настройки', settings['nrel_lang'])}
                    </SettingsText>
                    <WrapperSettings>
                        <Setting>
                            <p style={{ fontSize: getFontSizeFromSettings(settingFontSize) }}>
                                {translateWord('Тема', settings['nrel_lang'])}
                            </p>
                            <Select onChange={(e) => changeTheme(e)} value={settingTheme}>
                                <option value="dark">{translateWord('Темная', settings['nrel_lang'])}</option>
                                <option value="light">{translateWord('Светлая', settings['nrel_lang'])}</option>
                            </Select>
                        </Setting>
                        <Setting>
                            <p style={{ fontSize: getFontSizeFromSettings(settingFontSize) }}>
                                {translateWord('Размер текста', settings['nrel_lang'])}
                            </p>
                            <Select onChange={(e) => changeFontSize(e)} value={settingFontSize}>
                                <option value="small">{translateWord('Мелкий', settings['nrel_lang'])}</option>
                                <option value="medium">{translateWord('Средний', settings['nrel_lang'])}</option>
                                <option value="big">{translateWord('Большой', settings['nrel_lang'])}</option>
                            </Select>
                        </Setting>
                        <Setting>
                            <p style={{ fontSize: getFontSizeFromSettings(settingFontSize) }}>
                                {translateWord('Цвет интерфейса', settings['nrel_lang'])}
                            </p>
                            <input type="color" onChange={(e) => changeAccentColor(e)} value={settingAccentColor} />
                        </Setting>
                        <Setting>
                            <p style={{ fontSize: getFontSizeFromSettings(settingFontSize) }}>
                                {translateWord('Язык', settings['nrel_lang'])}
                            </p>
                            <Select onChange={(e) => changeLang(e)} value={settingLang}>
                                <option value="ru">Русский</option>
                                <option value="en">English</option>
                            </Select>
                        </Setting>

                        <SaveSettings
                            style={
                                settingTheme == 'light'
                                    ? { background: 'rgb(51, 47, 47)' }
                                    : { background: settingAccentColor }
                            }
                            onClick={saveSettings}
                        >
                            {translateWord('Сохранить', settings['nrel_lang'])}
                        </SaveSettings>
                    </WrapperSettings>
                </Container>
            </Main>
        </>
    );
};

/*
<Setting>
    <p style={{ fontSize: getFontSizeFromSettings(settingFontSize) }}>Режим для людей с ограниченными возможностями</p>
    <input
        style={{ transform: 'scale(1.2)', accentColor: settingAccentColor }}
        type="checkbox"
        onChange={(e) => changeInvalideMode(e)}
        checked={settingInvalideMode == 'on' ? true : false}
    />
</Setting>;


*/
