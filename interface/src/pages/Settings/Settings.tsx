import React, { useEffect, useState, useReducer, ChangeEvent } from 'react';
import { WrapperSettings, Container, Setting, SettingsText, SaveSettings } from './styled';
import { client } from '@api';
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { ScTemplate, ScType } from 'ts-sc-client';
import { Redirect } from 'react-router';
import { checkUser } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import { getUserName } from '@api/sc/checkUser';

export const Settings = () => {
    // Get Cookies
    const cookie = new Cookie();
    const userAddr = cookie.get('userAddr') ? new ScAddr(parseInt(String(cookie.get('userAddr')))) : new ScAddr(0);
    const password = cookie.get('password');

    return (
        <Container>
            <SettingsText>Настройки</SettingsText>
            <WrapperSettings>
                <Setting>
                    <p>Тема</p>
                    <select>
                        <option value="dark">Темная</option>
                        <option value="light">Светлая</option>
                    </select>
                </Setting>
                <Setting>
                    <p>Размер текста</p>
                    <select>
                        <option value="small">Мелкий</option>
                        <option value="mediul">Средний</option>
                        <option value="big">Большой</option>
                    </select>
                </Setting>
                <Setting>
                    <p>Цвет интерфейса</p>
                    <input type="color" />
                </Setting>
                <Setting>
                    <p>Язык</p>
                    <select>
                        <option value="ru">Русский</option>
                        <option value="en">Английский</option>
                    </select>
                </Setting>
                <SaveSettings>Сохранить</SaveSettings>
            </WrapperSettings>
        </Container>
    );
};
