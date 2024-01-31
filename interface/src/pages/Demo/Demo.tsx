import { useCallback, useEffect, useState, Fragment } from 'react';
import { Wrapper, ChatWrapper, BackBtn } from './styled';
import { Message } from '@components/Chat/Message';
import { Chat } from '@components/Chat';
import { Date } from '@components/Chat/Date';
import { routes } from '@constants';
import { ScAddr } from 'ts-sc-client';
import { resolveUserAgent } from '@agents/resolveUserAgent';
import { useChat } from '@hooks/useChat';
import * as React from 'react';
import { checkUser, getUserName } from '@api/sc/checkUser';
import { Redirect } from 'react-router';
import Cookie from 'universal-cookie';
import { SC_WEB_URL } from '@constants';

export const Demo = () => {
    // Get Cookies
    const cookie = new Cookie();
    const userAddr = cookie.get('userAddr') ? new ScAddr(parseInt(String(cookie.get('userAddr')))) : new ScAddr(0);
    const password = cookie.get('password');

    const [user, setUser] = useState<ScAddr | null>(null);
    const [isLoading, setIsLoading] = useState(false);

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

    return (
        <>
            {redirectError ? <Redirect to={{ pathname: routes.LOGIN }} /> : ''}

            <Wrapper>
                <BackBtn href={routes.HOME}></BackBtn>
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
