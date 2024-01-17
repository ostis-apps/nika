import React, { useEffect, useState, useReducer } from 'react';
import {
    Circle,  
    Circle1,
    Wrapper,
    WrapperContent,
    Form, 
    FormText,
    Input,
    FormBtn,
    Link,
} from './styled'
import { client } from "@api";
import { ScAddr } from 'ts-sc-client';
import { routes } from '@constants';
import { ScTemplate, ScType } from 'ts-sc-client';

export const Login = () => {
    return (
        <div>
            <Circle></Circle>
            <Circle1></Circle1>
            <Wrapper>
                <WrapperContent>
                    <Form>
                        <FormText>Авторизация</FormText>
                        <Input type="email" name="email" placeholder="E-mail" required></Input>
                        <Input type="password" name="pass" id="pass" placeholder="Пароль" required></Input>
                        <FormBtn type="submit" id="submit">Войти</FormBtn>
                        <Link href={routes.REGISTRATION}>Еще нет аккаунта?</Link>
                    </Form>
                </WrapperContent>
            </Wrapper>
        </div>
    );
}