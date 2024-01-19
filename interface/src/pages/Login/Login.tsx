import React, { useEffect, useState, useReducer, ChangeEvent } from 'react';
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
    Error, 
} from './styled'
import { client } from "@api";
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScTemplate, ScType, ScLinkContentType, ScEventType, ScEventParams } from 'ts-sc-client';


export const Login = () => {
    const [pass, setPass] = useState<string>("");
    const [username, setUsername] = useState<string>("");
    const [errorPass, setErrorPass] = useState<boolean>(false);
    const [errorUserNotFound, setErrorUserNotFound] = useState<boolean>(false);
    const [errorEmpty, setErrorEmpty] = useState<boolean>(false);

    const loginUser = (name:string, password:string) => {
        // Here will be code with login user

        console.log(`Login user ${ name } with password: ${password}.`)
    }   
    
    const updateUsername = (e:ChangeEvent<HTMLInputElement>) => {
        setUsername(e.target.value);
    }

    const updatePass = (e:ChangeEvent<HTMLInputElement>) => {
        setPass(e.target.value);
    }

    const check = (e) => {
        e.preventDefault();

        setErrorEmpty(false);
        setErrorUserNotFound(false);
        setErrorPass(false);

        // Need to write code for check other errors
        if (username == '' || pass == '') {
            setErrorEmpty(true);
            return 1;
        } 
        
        
        loginUser(username, pass);
        
    }

    return (
        <div>
            <Circle></Circle>
            <Circle1></Circle1>
            <Wrapper>
                <WrapperContent>
                    <Form>
                        <FormText>Авторизация</FormText>
                        <Input type="text" name="username" placeholder="Имя пользователя" onChange={ updateUsername } required></Input>
                        <Input type="password" name="pass" id="pass" placeholder="Пароль" onChange={ updatePass } required></Input>
                        {errorEmpty ? (<Error>Поля должны быть заполнены!</Error>) : ""}
                        <FormBtn type="submit" id="submit" onClick={ check }>Войти</FormBtn>
                        <Link href={ routes.REGISTRATION }>Еще нет аккаунта?</Link>
                    </Form>
                </WrapperContent>
            </Wrapper>
        </div>
    );
}