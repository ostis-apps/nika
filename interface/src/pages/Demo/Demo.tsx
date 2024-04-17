import { useCallback, useEffect, useState, Fragment } from 'react';
import { Wrapper, ChatWrapper, SCgViewerWrapper, PopupWrapper} from "./styled";
import { Message } from '@components/Chat/Message';
import { Chat } from '@components/Chat';
import { Date } from '@components/Chat/Date';
import { ScAddr } from 'ts-sc-client';
import { resolveUserAgent } from '@agents/resolveUserAgent';
import { createPopupCheck } from '@agents/helper';
import { useChat } from '@hooks/useChat';
import { SC_WEB_URL } from "@constants";
import { ScClient } from 'ts-sc-client';
import { SC_URL } from '@constants';
import { AddRelationToEntityPopup, CreateClassInstancePopup, CreateMessageClassPopup, CreatePhraseTemplatePopup, CreateClassPopup, CreateRelationPopup } from './Popups';

const client = new ScClient(SC_URL);


export const Demo = () => {
    const [user, setUser] = useState<ScAddr | null>(null);
    const [isLoading, setIsLoading] = useState(false);

    const [createMessageClassAndPhraseTemplatePopup, setCreateMessageClassAndPhraseTemplatePopup] = useState(false);
    const [createClassPopup, setCreateClassPopup] = useState(false);
    const [createPhraseTemplatePopup, setCreatePhraseTemplatePopup] = useState(false);
    const [createClassInstancePopup, setCreateClassInstancePopup] = useState(false);
    const [createRelationToEntityPopup, setCreateRelationToEntityPopup] = useState(false);
    const [createRelationPopup, setCreateRelationPopup] = useState(false);
    const [form, setForm] = useState("");
    const [relationForm, setRelationForm] = useState<string[]>([]);   

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
            
            createPopupCheck(setCreateMessageClassAndPhraseTemplatePopup, 'concept_popup_component_for_creating_message_class_and_phrase_template');
            createPopupCheck(setCreateClassInstancePopup, 'concept_popup_component_for_creating_class_instance');
            createPopupCheck(setCreateRelationPopup, 'concept_popup_component_for_creating_relation');
            createPopupCheck(setCreateClassPopup, 'concept_popup_component_for_creating_class');
        })();
    }, [initChat]);

    const MessageClassPopup = ()  => { return CreateMessageClassPopup(
        setCreateMessageClassAndPhraseTemplatePopup,
        setCreatePhraseTemplatePopup,
        setForm);
    };

    const PhraseTemplatePopup = () => { return CreatePhraseTemplatePopup(
        setCreatePhraseTemplatePopup,
        form);
    };

    const ClassInstancePopup = () => { return CreateClassInstancePopup(setCreateClassInstancePopup, setCreateRelationToEntityPopup, setRelationForm)};

    const RelationToEntityPopup = () => { return AddRelationToEntityPopup(setCreateRelationToEntityPopup, relationForm)};

    const ClassPopup = () => { return CreateClassPopup(setCreateClassPopup)};

    const RelationPopup = () => {return CreateRelationPopup(setCreateRelationPopup)};

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
            {createMessageClassAndPhraseTemplatePopup && (
                <PopupWrapper>
                    <MessageClassPopup />
                </PopupWrapper>
                )
            }
            {createPhraseTemplatePopup && (
                <PopupWrapper>
                    <PhraseTemplatePopup />
                </PopupWrapper>
                )
            }
            {createClassInstancePopup && (
                <PopupWrapper>
                    <ClassInstancePopup />
                </PopupWrapper>
                )
            }
            {createRelationToEntityPopup && (
                <PopupWrapper>
                    <RelationToEntityPopup />
                </PopupWrapper>
                )
            }
            {createClassPopup && (
                <PopupWrapper>
                    <ClassPopup />
                </PopupWrapper>
                )
            }
            {createRelationPopup && (
                <PopupWrapper>
                    <RelationPopup />
                </PopupWrapper>
                )
            }
            
        </Wrapper>
    );
};
