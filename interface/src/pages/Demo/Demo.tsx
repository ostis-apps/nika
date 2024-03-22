import { useCallback, useEffect, useState, Fragment } from 'react';
import { Wrapper, ChatWrapper, SCgViewerWrapper, PopupWrapper} from "./styled";
import { Message } from '@components/Chat/Message';
import { Chat } from '@components/Chat';
import { Date } from '@components/Chat/Date';
import { ScAddr } from 'ts-sc-client';
import { resolveUserAgent } from '@agents/resolveUserAgent';
import { createClassPopupCheck, createPopupCheck, failedToCreateConstruction } from '@agents/helper';
import { useChat } from '@hooks/useChat';
import { SC_WEB_URL } from "@constants";
import { ScClient } from 'ts-sc-client';
import { SC_URL } from '@constants';
import { CreateQuestionClassPopup, CreatePhraseTemplatePopup, CreateClassPopup } from './Popups';

const client = new ScClient(SC_URL);


export const Demo = () => {
    const [user, setUser] = useState<ScAddr | null>(null);
    const [isLoading, setIsLoading] = useState(false);

    const [createPopup, setCreatePopup] = useState(false);
    const [createPhraseTemplatePopup, setCreatePhraseTemplatePopup] = useState(false);
    const [createClassPopup, setCreateClassPopup] = useState(false);
    const [form, setForm] = useState("");
    const [isVisible, setIsVisible] = useState(false);
    const [createClass, setCreateClass] = useState(false);  
    const [errorLabel, setErrorLabel] = useState(false);
    const [closeLabel, setCloseLabel] = useState(false);    
    
    if(errorLabel){setTimeout(() => {setErrorLabel(false)}, 7000);}

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
            createPopupCheck(setCreatePopup);
            createClassPopupCheck(setCreateClassPopup);
            failedToCreateConstruction(setErrorLabel);
        })();
    }, [initChat]);

    const QuestionClassPopup = ()  => { return CreateQuestionClassPopup(
        setCreatePopup,
        setCreatePhraseTemplatePopup,
        setForm,
        setCloseLabel);
    };

    const PhraseTemplatePopup = () => { return CreatePhraseTemplatePopup(
        setCreatePhraseTemplatePopup,
        form,
        setIsVisible,
        setCloseLabel);
    };
    const ClassPopup = () => { return CreateClassPopup(
        setCreateClassPopup,
        setCreateClass,
        setCloseLabel);
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
                    <QuestionClassPopup />
                </PopupWrapper>
                )
            }
            {createPhraseTemplatePopup && (
                <PopupWrapper>
                    <PhraseTemplatePopup />
                </PopupWrapper>
                )
            }
            {createClassPopup && (
                <PopupWrapper>
                    <ClassPopup />
                </PopupWrapper>
            )}
            <div className={`fade-in-out ${isVisible ? 'visible' : 'hidden'}`}>
                <p className='finish label'>Теперь я умею отвечать на новый вопрос!</p>
            </div>

            <div className={`fade-in-out ${createClass ? 'visible' : 'hidden'}`}>
                <p className='finish label'>Класс успешно создан!</p>
            </div>

            <div className={`fade-in-out ${errorLabel ? 'visible' : 'hidden'}`}>
                <p className='error label'>Что-то пошло не так. <br /> Пожалуйста, попробуйте снова.</p>
            </div>

            <div className={`fade-in-out ${closeLabel ? 'visible' : 'hidden'}`}>
                <p className='close label'>Действие завершено пользователем.</p>
            </div>
        </Wrapper>
    );
};
