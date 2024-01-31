import React, { useEffect, useState, useReducer, ChangeEvent } from 'react';
import { WrapperSettings, Container, Setting, SettingsText, SaveSettings } from './styled';
import { client } from '@api';
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { ScTemplate, ScType } from 'ts-sc-client';
import { Redirect } from 'react-router';
import { checkUser } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import { getUserName, findSettings, updateSettings, getUserSettings } from '@api/sc/checkUser';
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

        location.reload();
    };

    const dark = `
        background: black;
        h1 {
            color: white;
        }
    `;

    const light = `
        background: white;
        * {
            color: black;
        }
    `;

    const Main = styled.div`
        display: flex;
        width: 100%;
        height: 100%;
        ${settings['nrel_theme'] == 'dark' ? dark : light};
    `;

    return (
        <Main>
            {savedSettings ? <Redirect to={{ pathname: routes.HOME }} /> : ''}
            {redirectError ? <Redirect to={{ pathname: routes.LOGIN }} /> : ''}

            <Container>
                <SettingsText>Настройки</SettingsText>
                <WrapperSettings>
                    <Setting>
                        <p>Тема</p>
                        <select onChange={(e) => changeTheme(e)} value={settingTheme}>
                            <option value="dark">Темная</option>
                            <option value="light">Светлая</option>
                        </select>
                    </Setting>
                    <Setting>
                        <p>Размер текста</p>
                        <select onChange={(e) => changeFontSize(e)} value={settingFontSize}>
                            <option value="small">Мелкий</option>
                            <option value="medium">Средний</option>
                            <option value="big">Большой</option>
                        </select>
                    </Setting>
                    <Setting>
                        <p>Цвет интерфейса</p>
                        <input type="color" onChange={(e) => changeAccentColor(e)} value={settingAccentColor} />
                    </Setting>
                    <Setting>
                        <p>Язык</p>
                        <select onChange={(e) => changeLang(e)} value={settingLang}>
                            <option value="ru">Русский</option>
                            <option value="en">Английский</option>
                        </select>
                    </Setting>
                    <Setting>
                        <p>Режим для людей с ограниченными возможностями</p>
                        <input
                            type="checkbox"
                            onChange={(e) => changeInvalideMode(e)}
                            checked={settingInvalideMode == 'on' ? true : false}
                        />
                    </Setting>
                    <SaveSettings onClick={saveSettings}>Сохранить</SaveSettings>
                </WrapperSettings>
            </Container>
        </Main>
    );
};
