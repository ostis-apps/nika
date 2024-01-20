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
import { Redirect } from 'react-router';


export const Login = () => {
    if (document.cookie != '') 
        return (
            <div>
                <Redirect to={{ pathname: routes.HOME }}/>
            </div>
        )
        
    const [userAuthorised, setUserAuthorised] = useState<boolean>(false); 
    const [pass, setPass] = useState<string>("");
    const [username, setUsername] = useState<string>("");
    const [errorPass, setErrorPass] = useState<boolean>(false);
    const [errorUserNotFound, setErrorUserNotFound] = useState<boolean>(false);
    const [errorEmpty, setErrorEmpty] = useState<boolean>(false);

    const onAuthorisedResult = async (addr:ScAddr, edgeAddr:ScAddr, edgeToResultAddr:ScAddr)=> {
        let template = new ScTemplate();
        template.triple(
            ScType.NodeVar,
            edgeToResultAddr, 
            [ScType.NodeVar, '_result'],
        )
        template.triple(
            '_result',
            ScType.EdgeAccessVarPosPerm,
            [ScType.NodeVar, '_user'],
        )
        const result = await client.templateSearch(template);
        if (result.length > 0) {
            // loginUser(result);
            setUserAuthorised(true);
        }
    }

    // Need to write this code for authorise user 
    
    useEffect(() =>  { 
        (async () => {
            const baseKeynodes = [
                { id: "nrel_answer", type: ScType.NodeConstNoRole },
            ];
            const keynodes = await client.resolveKeynodes(baseKeynodes);

            const eventParams = new ScEventParams(keynodes['nrel_answer'], ScEventType.AddOutgoingEdge, onAuthorisedResult);
            await client.eventsCreate([eventParams]); 
        })();
    }, [])
    

    const loginUser = async (name:string, password:string) => {
        const baseKeynodes = [
            { id: "action_authorise_user", type: ScType.NodeConstClass },
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
        construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['action_authorise_user'], "action_node");
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
            { userAuthorised ? <Redirect to={{ pathname: routes.HOME }}/> : (
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
            )}
        </div>
    );
}