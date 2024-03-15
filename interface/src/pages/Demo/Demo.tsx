import { useCallback, useEffect, useState, Fragment } from 'react';
import { Wrapper, ChatWrapper, SCgViewerWrapper, PopupWrapper} from "./styled";
import { Message } from '@components/Chat/Message';
import { Chat } from '@components/Chat';
import { Date } from '@components/Chat/Date';
import { ScAddr, ScConstruction, ScEventParams, ScEventType, ScLinkContent, ScLinkContentType, ScTemplate, ScType } from 'ts-sc-client';
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
    const [createPopup, setCreatePopup] = useState(false);
    const [createPhraseTemplatePopup, setCreatePhraseTemplatePopup] = useState(false);

    const phraseSystemIdentifierRef = useRef<HTMLInputElement>(null);
    const phraseRussianIdentifierRef = useRef<HTMLInputElement>(null);
    const questionSystemIdentifierRef = useRef<HTMLInputElement>(null);
    const questionRussianIdentifierRef = useRef<HTMLInputElement>(null);
    const questionWitAiRef = useRef<HTMLInputElement>(null);


    const createPopupCheck = async ()  => {
        const concept_popup = 'concept_popup';

        const baseKeynodes = [
            { id: concept_popup, type: ScType.NodeConstClass},
        ];

        const keynodes = await client.resolveKeynodes(baseKeynodes);
        const eventParams = new ScEventParams(keynodes[concept_popup], ScEventType.AddOutgoingEdge, () => {setCreatePopup(true)});
        await client.eventsCreate([eventParams])
    }

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
            createPopupCheck();
        })();
    }, [initChat]);


    const openFirstWindow: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
        setCreatePhraseTemplatePopup(false);
        setCreatePopup(true);
      };

    const openSecondWindow: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
        setCreatePopup(false);
        setCreatePhraseTemplatePopup(true);
    };


    const CreatePhraseTemplatePopup = () => {
        const [chipsValues, setChipsValues] = useState<string[]>([]);
        const [editingIndex, setEditingIndex] = useState(null);
        const handleChipAdd = (value: string) => {
            setChipsValues([...chipsValues, value]);
        };

        useEffect(() => {
            phraseSystemIdentifierRef.current!.value = "concept_phrase_template_";
            phraseRussianIdentifierRef.current!.value = "Шаблон фразы о ";
        }, [phraseSystemIdentifierRef, phraseRussianIdentifierRef])
        
        const handleChipDelete = (index: number) => {
          const newChipsValues = [...chipsValues];
          newChipsValues.splice(index, 1);
          setChipsValues(newChipsValues);
        };

        const handleChipEdit = (index) => {
            setEditingIndex(index);
          };
        
        const handleChipChange = (event, index) => {
          const updatedChipsValues = [...chipsValues];
          updatedChipsValues[index] = event.target.value;
          setChipsValues(updatedChipsValues);
        };

        const handleClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
            await handleSave(phraseSystemIdentifierRef, phraseRussianIdentifierRef, chipsValues);
          };

        const closeClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
          setCreatePhraseTemplatePopup(false);
        };

        const systemIdtfHandleKeyDown = (event) => {
            const inputValue = phraseSystemIdentifierRef.current?.value;
            const fixedText = "concept_phrase_template_";

            if (inputValue === "concept_phrase_template_" && (event.key === 'Backspace' || event.key === 'Delete')) {
            event.preventDefault();
            phraseSystemIdentifierRef.current!.value = fixedText;
            }  
        }

        const russianIdtfHandleKeyDown = (event) => {
            const inputValue = phraseRussianIdentifierRef.current?.value;
            const fixedText = "Шаблон фразы о ";

            if (inputValue === fixedText && (event.key === 'Backspace' || event.key === 'Delete')) {
            event.preventDefault();
            phraseRussianIdentifierRef.current!.value = fixedText;
            }  
        }

        return (
          <div className="popup">
            <h2>Создание сообщения и шаблонов ответов</h2>
            <button className='close_button' onClick={closeClick}>×</button>
            <div className='numdiv'>
                <button className='p1' onClick={openFirstWindow}>1</button>
                <button className='p2'>2</button>
            </div>
            <div className="form">
                <h4>Системный индетификатор</h4>
                <input
                type="text"
                className="input"
                ref={phraseSystemIdentifierRef}
                onKeyDown={systemIdtfHandleKeyDown}
                />
                <h4>Индетификатор на русском языке</h4>
                <input
                type="text"
                className="input"
                ref={phraseRussianIdentifierRef}
                onKeyDown={russianIdtfHandleKeyDown}
                />
                </div>
                <h2>Шаблоны ответов</h2>
                <div className="chips">
                    <input
                        type="text"
                        className="chip-input"
                        placeholder="Шаблон фразы"
                        onKeyDown={(e: React.KeyboardEvent<HTMLInputElement>) => {
                            if (e.key === 'Enter') {
                            handleChipAdd(e.currentTarget.value);
                            e.currentTarget.value = '';
                            }
                        }}
                    />
                    {chipsValues.map((value, index) => (
                        <div className="chip-div" key={index}>
                        {editingIndex === index ? (
                            <input
                            type="text"
                            className="chip"
                            value={value}
                            onChange={(e) => handleChipChange(e, index)}
                            onBlur={() => setEditingIndex(null)}
                            autoFocus
                            />
                        ) : (
                            <div className="chip" onClick={() => handleChipEdit(index)}>
                            {value}
                            </div>
                        )}
                        <span className="chip-delete" onClick={() => handleChipDelete(index)}>
                            X
                        </span>
                        </div>
                    ))}
                </div>
                <button className="button save" onClick={handleClick}>
                Сохранить
                </button>
            
          </div>
        );
      };


      const CreateQuestionClassPopup = () => {

        useEffect(() => {
            questionSystemIdentifierRef.current!.value = "concept_message_about_";
            questionRussianIdentifierRef.current!.value = "Сообщение о ";
        }, [questionSystemIdentifierRef, questionRussianIdentifierRef])

        const handleClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
            setCreatePopup(false);
            setCreatePhraseTemplatePopup(true);
          };

        const closeClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
          setCreatePopup(false);
        };

        const systemIdtfHandleKeyDown = (event) => {
            const inputValue = questionSystemIdentifierRef.current?.value;
            const fixedText = "concept_message_about_";

            if (inputValue === fixedText && (event.key === 'Backspace' || event.key === 'Delete')) {
            event.preventDefault();
            questionSystemIdentifierRef.current!.value = fixedText;
            }  
        }

        const russianIdtfHandleKeyDown = (event) => {
            const inputValue = questionRussianIdentifierRef.current?.value;
            const fixedText = "Сообщение о ";

            if (inputValue === fixedText && (event.key === 'Backspace' || event.key === 'Delete')) {
            event.preventDefault();
            questionRussianIdentifierRef.current!.value = fixedText;
            }  
        }

        return (
          <div className="popup">
            <h2>Создание сообщения и шаблонов ответов</h2>
            <button className='close_button' onClick={closeClick}>×</button>
            <div className='numdiv'>
                <button className='p3'>1</button>
                <button className='p4' onClick={openSecondWindow}>2</button>
            </div>
            <div className="form">
                <h4>Системный индетификатор</h4>
                <input
                type="text"
                className="input"
                ref={questionSystemIdentifierRef}
                onKeyDown={systemIdtfHandleKeyDown}
                />
                <h4>Индетификатор на русском языке</h4>
                <input
                type="text"
                className="input"
                ref={questionRussianIdentifierRef}
                onKeyDown={russianIdtfHandleKeyDown}
                />
                <h4>Название интента в Wit.ai</h4>
                <input
                type="text"
                className="input"
                ref={questionWitAiRef}
                placeholder='your_name'
                />
                </div>
                <button className="button-save" onClick={handleClick}>
                Далее
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
            {createPopup && (
                <PopupWrapper>
                    <CreateQuestionClassPopup />
                </PopupWrapper>
                )
            }
            {createPhraseTemplatePopup && (
                <PopupWrapper>
                    <CreatePhraseTemplatePopup />
                </PopupWrapper>
                )
            }
        </Wrapper>
    );
};
