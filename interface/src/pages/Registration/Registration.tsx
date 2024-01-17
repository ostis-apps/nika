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
    Error,
    Link,
} from './styled'
import { client } from "@api";
import { routes } from '@constants';
import { ScAddr, ScConstruction, ScLinkContent, ScTemplate, ScType, ScLinkContentType, ScEventType, ScEventParams } from 'ts-sc-client';
import Password from 'antd/lib/input/Password';

export const Registration = () => {
    const [pass, setPass] = useState<string>("");
    const [repeatPass, setRepeatPass] = useState<string>("");
    const [username, setUsername] = useState<string>("");
    const [errorNotEqual, setErrorNotEqual] = useState<boolean>(false);
    const [errorPassLength, setErrorPassLength] = useState<boolean>(false);
    const [errorUsernameLength, setErrorUsernameLength] = useState<boolean>(false);

    const onRegisterResult = (addr:ScAddr, edgeAddr:ScAddr, resultAddr:ScAddr)=> {
        let template = new ScTemplate();
        template.triple(
            resultAddr, 
            ScType.EdgeAccessVarPosPerm, 
            ScType.NodeVar,
        )
    }

    useEffect(async () => {
        const baseKeynodes = [
            { id: "nrel_answer", type: ScType.NodeConstNoRole },
        ];
        const keynodes = await client.resolveKeynodes(baseKeynodes);

        const eventParams = new ScEventParams(keynodes['nrel_answer'], ScEventType.AddOutgoingEdge, onRegisterResult);
        await client.eventsCreate([eventParams]); 
    }, [])


    const registerUser = async (name:string, password:string) => {
        const baseKeynodes = [
            { id: "action_register_user", type: ScType.NodeConstClass },
            { id: "question_initiated", type: ScType.NodeConstClass },
            { id: "question", type: ScType.NodeConstClass },
        ];

        const helpKeynodes = [
            { id: "rrel_1", type: ScType.NodeConstRole },
            { id: "rrel_2", type: ScType.NodeConstRole },
        ];
    
        const keynodes = await client.resolveKeynodes(baseKeynodes);
        const hKeynodes = await client.resolveKeynodes(helpKeynodes);
        
        const construction = new ScConstruction();
        construction.createNode(ScType.NodeConst, "action_node")
        construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['action_register_user'], "action_node");
        construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['question_initiated'], "action_node");
        construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['question'], "action_node");
        construction.createLink(ScType.LinkConst, new ScLinkContent(name, ScLinkContentType.String), "username");
        construction.createLink(ScType.LinkConst, new ScLinkContent(password, ScLinkContentType.String), "password");
        construction.createEdge(ScType.EdgeAccessConstPosPerm, "action_node", "username", "name_edge");
        construction.createEdge(ScType.EdgeAccessConstPosPerm, "action_node", "password", "pass_edge");
        construction.createEdge(ScType.EdgeAccessConstPosPerm, hKeynodes['rrel_1'], "name_edge");
        construction.createEdge(ScType.EdgeAccessConstPosPerm, hKeynodes['rrel_2'], "pass_edge");
        
        await client.createElements(construction);


    }

    

    const check = (e) => {
        e.preventDefault()

        setErrorNotEqual(false);
        setErrorPassLength(false);

        if (username.length < 3) {
            setErrorUsernameLength(true);
            return 1;
        } else if (pass !== repeatPass)  {
            setErrorNotEqual(true);
            return 1;
        } else if (pass.length < 6) {
            setErrorPassLength(true);
            return 1;
        } 
        
        registerUser(username, pass)
    };

    const updateChangePassword = (e:ChangeEvent<HTMLInputElement>) => {
        setPass(e.target.value);
    };

    const updateChangeRepeatPassword = (e:ChangeEvent<HTMLInputElement>) => {
        setRepeatPass(e.target.value);
    };

    const updateUsername = (e:ChangeEvent<HTMLInputElement>) => {
        setUsername(e.target.value);
    };

    return (
        <div>
            <Circle></Circle>
            <Circle1></Circle1>
            <Wrapper>
                <WrapperContent>
                    <Form>
                        <FormText>Регистрация</FormText>
                        <Input type="string" name="username" placeholder="Имя пользователя" onChange={updateUsername} required></Input>
                        {errorUsernameLength ? (<Error>Имя пользователя должно состоять как минимум из 4 символов.</Error>) : ""}
                        <Input type="password" name="pass1" id="pass1" placeholder="Пароль" onChange={updateChangePassword} required></Input>
                        {errorPassLength ? (<Error>Пароль должен состоять как минимум из 6 символов.</Error>) : ""}
                        <Input type="password" name="pass2" id="pass2" placeholder="Повторите пароль" onChange={updateChangeRepeatPassword} required></Input>
                        {errorNotEqual ? (<Error>Пароли не совпадают</Error>) : ""}
                        <FormBtn type="submit" id="submit" key="1"  onClick={check}>Войти</FormBtn>
                        <Link href={routes.LOGIN}>Уже есть аккаунт?</Link>
                    </Form>
                </WrapperContent>
            </Wrapper>
        </div>
    );
}