import { useCallback, useEffect, useState, Fragment } from 'react';
import { Wrapper, ChatWrapper, SCgViewerWrapper, PopupWrapper} from "./styled";
import { Message } from '@components/Chat/Message';
import { Chat } from '@components/Chat';
import { Date } from '@components/Chat/Date';
import { ScAddr, ScConstruction, ScLinkContent, ScLinkContentType, ScTemplate, ScType } from 'ts-sc-client';
import { resolveUserAgent } from '@agents/resolveUserAgent';
import { handleSave } from '@agents/helper';
import { useChat } from '@hooks/useChat';
import * as React from "react";
import { useRef } from 'react';
import { SC_WEB_URL } from "@constants";
import { ScClient } from 'ts-sc-client';
import { SC_URL } from '@constants';
const client = new ScClient(SC_URL);


export const Demo = () => {
    const [user, setUser] = useState<ScAddr | null>(null);
    const [isLoading, setIsLoading] = useState(false);

    const systemIdentifierRef = useRef<HTMLInputElement>(null);
    const russianIdentifierRef = useRef<HTMLInputElement>(null);
    const englishIdentifierRef = useRef<HTMLInputElement>(null);
    const answerTemplatesRef = useRef<HTMLTextAreaElement>(null);

    const { initChat, sendMessage, isAgentAnswer, onFetching, messages, chatRef } = useChat(user);
    const onSend = useCallback(
        async (text: string) => {
            if (!user) return;
            await sendMessage(user, text);
        },
        [user, sendMessage],
    );

    const url = SC_WEB_URL + '/?sys_id=answer_structure&scg_structure_view_only=true';

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


    const Popup = () => {
        const [chipsValues, setChipsValues] = useState<string[]>([]);
        const handleChipAdd = (value: string) => {
            setChipsValues([...chipsValues, value]);
        };
        
        const handleChipDelete = (index: number) => {
          const newChipsValues = [...chipsValues];
          newChipsValues.splice(index, 1);
          setChipsValues(newChipsValues);
        };

        const handleClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
            await handleSave(systemIdentifierRef, russianIdentifierRef, englishIdentifierRef, chipsValues);
          };

        return (
          <div className="popup">
            <h2>Создание шаблона ответов</h2>
            <input
              type="text"
              className="input"
              placeholder="Системный индетификатор"
              ref={systemIdentifierRef}
            />
            <input
              type="text"
              className="input"
              placeholder="Индетификатор на русском"
              ref={russianIdentifierRef}
            />
            <input
              type="text"
              className="input"
              placeholder="Индетификатор на английском"
              ref={englishIdentifierRef}
            />
            <div className="chips">
                {chipsValues.map((value, index) => (
                <div className='chip-div'>
                <div className="chip" key={index}>
                    {value}
                </div>
                <span
                className="chip-delete"
                onClick={() => handleChipDelete(index)}
                >
                X
                </span>
                </div>
                ))}
                <input
                type="text"
                className="chip-input"
                placeholder="Шаблоны ответов*"
                onKeyDown={(e: React.KeyboardEvent<HTMLInputElement>) => {
                    if (e.key === 'Enter') {
                    handleChipAdd(e.currentTarget.value);
                    e.currentTarget.value = '';
                    }
                }}
                />
            </div>
            <button className="button save" onClick={handleClick}>
              Сохранить
            </button>
          </div>
        );
      };

    return (
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
                                        <div dangerouslySetInnerHTML={{__html: item.text}} />
                    
                                    ) : (
                                    <div>{item.text}</div>
                                    )}

                                </Message>
                            </Fragment>
                        );
                    })}
                </Chat>
            </ChatWrapper>
            <SCgViewerWrapper>
                <iframe src={url} style={{width: '100%', height: '100%', border: 0, borderRadius: '15px'}}/>
            </SCgViewerWrapper>
            {messages[messages?.length - 1]?.text === 'Создать шаблон ответа' && (
                <PopupWrapper>
                    <Popup /> {/* Ваш компонент всплывающего окна */}
                </PopupWrapper>
                )
            }
        </Wrapper>
    );
};
