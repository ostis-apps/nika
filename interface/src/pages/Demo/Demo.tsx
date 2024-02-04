import { useCallback, useEffect, useState, Fragment } from 'react';
import { Wrapper, ChatWrapper, BackBtn, Linktitle, NavLink } from './styled';
import { Message } from '@components/Chat/Message';
import { Chat } from '@components/Chat';
import { Date } from '@components/Chat/Date';
import { routes } from '@constants';
import { ScAddr } from 'ts-sc-client';
import { resolveUserAgent } from '@agents/resolveUserAgent';
import { useChat } from '@hooks/useChat';
import * as React from 'react';
import { checkUser, getUserName, getUserSettings } from '@api/sc/checkUser';
import { Redirect } from 'react-router';
import Cookie from 'universal-cookie';
import { SC_WEB_URL } from '@constants';
import styled from 'styled-components';

export const Demo = () => {
    // Get Cookies
    const cookie = new Cookie();
    const userAddr = cookie.get('userAddr') ? new ScAddr(parseInt(String(cookie.get('userAddr')))) : new ScAddr(0);
    const password = cookie.get('password');

    const [user, setUser] = useState<ScAddr | null>(null);
    const [isLoading, setIsLoading] = useState(false);
    const [userTheme, setUserTheme] = useState<String>('dark');

    const { initChat, sendMessage, isAgentAnswer, onFetching, messages, chatRef } = useChat(user);
    const onSend = useCallback(
        async (text: string) => {
            if (!user) {
                return;
            }
            await sendMessage(user, text);
        },
        [user, sendMessage],
    );

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
            } else {
                return 1;
            }
        } else {
            setRedirectError(true);
        }
    };

    useEffect(() => {
        (async () => {
            check();
        })();
    }, []);

    useEffect(() => {
        (async () => {
            const userSettings = await getUserSettings(userAddr);
            setUserTheme(userSettings['nrel_theme']);
        })();
    }, []);

    useEffect(() => {
        (async () => {
            if ((await check()) == 1) {
                setIsLoading(true);
                const user = await resolveUserAgent();
                if (!user) return;
                setUser(user);
                await initChat([user]);
                setIsLoading(false);
            }
        })();
    }, [initChat]);

    const Arrow = styled.div`
        ${userTheme == 'dark' ? 'border: solid #413d3d' : 'border: solid white'};
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

    return (
        <>
            {redirectError ? <Redirect to={{ pathname: routes.LOGIN }} /> : ''}
            <NavLink
                href={routes.HOME}
                style={userTheme == 'dark' ? { background: '#413d3d' } : { background: 'white' }}
                className="nav"
            >
                <Arrow
                    style={
                        userTheme == 'dark'
                            ? { border: 'solid white', borderWidth: '0 3px 3px 0' }
                            : { border: 'solid #413d3d', borderWidth: '0 3px 3px 0' }
                    }
                ></Arrow>
                <Linktitle style={userTheme == 'dark' ? { color: 'white' } : { color: 'black' }} className="title">
                    Назад
                </Linktitle>
            </NavLink>
            <Wrapper>
                <ChatWrapper>
                    <Chat
                        ref={chatRef}
                        isLoading={isLoading}
                        onSend={onSend}
                        onFetching={onFetching}
                        isAgentAnswer={isAgentAnswer}
                    >
                        {messages.map((item, ind) => {
                            const prevItem = messages[ind - 1];
                            const showDate = item.date !== prevItem?.date;
                            return (
                                <Fragment key={item.id}>
                                    {showDate && <Date date={item.date} />}
                                    <Message
                                        isLeft={!!user && !item.author.equal(user)}
                                        time={item.time}
                                        isLoading={item.isLoading}
                                    >
                                        {typeof item.text === 'string' ? (
                                            <div dangerouslySetInnerHTML={{ __html: item.text }} />
                                        ) : (
                                            <div>{item.text}</div>
                                        )}
                                    </Message>
                                </Fragment>
                            );
                        })}
                    </Chat>
                </ChatWrapper>
            </Wrapper>
        </>
    );
};
