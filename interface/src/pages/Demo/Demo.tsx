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
import { SC_WEB_URL } from '@constants';

export const Demo = () => {
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

    useEffect(() => {
        (async () => {
            setIsLoading(true);
            const user = await resolveUserAgent();
            if (!user) return;
            setUser(user);
            await initChat([user]);
            setIsLoading(false);
        })();
    }, [initChat]);

    return (
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
    );
};
