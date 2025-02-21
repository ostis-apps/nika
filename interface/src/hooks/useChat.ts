import { useCallback, useEffect, useRef, useState } from 'react';
import { ScAddr, ScEventSubscriptionParams, ScEventType, ScType } from 'ts-sc-client';
import { client } from '@api';
import { dialogAgent } from '@api/sc/agents/dialogAgent';
import { getInfoMessage, searchChatMessages } from '@api/sc/search/searchChatMessages';
import { newMessageAgent, generateLinkText } from '@api/sc/agents/newMessageAgent';

interface IMessage {
    addr: ScAddr;
    text: string | number;
    id: number;
    time: string | number;
    date: string;
    author: ScAddr;
    isLoading?: boolean;
}

export const useChat = (user: ScAddr | null) => {
    const [messages, setMessages] = useState<IMessage[]>([]);
    const [chatNode, setChatNode] = useState<ScAddr | null>(null);
    const [shouldEnd, setShouldEnd] = useState(false);
    const [isAgentAnswer, setIsAgentAnswer] = useState(false);

    const chatRef = useRef<HTMLDivElement>(null);

    const subscribeToMessage = useCallback(async () => {
        if (!chatNode) return;

        const nrelAuthors = 'nrel_authors';
        const nrelScTextTranslation = 'nrel_sc_text_translation';

        const baseKeynodes = [
            { id: nrelAuthors, type: ScType.ConstNodeNonRole },
            { id: nrelScTextTranslation, type: ScType.ConstNodeNonRole },
        ];

        const keynodes = await client.resolveKeynodes(baseKeynodes);

        const onActionFinished = async (_subscribedAddr: ScAddr, _arc: ScAddr, messageNode: ScAddr) => {
            if (!messageNode.isValid()) return;
            const newMessage = await getInfoMessage(messageNode, keynodes);

            if (!newMessage || !user) return;

            setMessages((prev) => {
                return [...prev.filter((el) => el.id !== newMessage.id), newMessage];
            });
            if (newMessage.author.equal(user)) setIsAgentAnswer(true);
        };
        const eventParams = new ScEventSubscriptionParams(chatNode, ScEventType.AfterGenerateOutgoingArc, onActionFinished);
        await client.createElementaryEventSubscriptions([eventParams]);
    }, [chatNode, user]);

    const minNumberMessages = () => {
        const MIN_MESSAGE_HEIGHT = 60;

        if (!chatRef.current) return 0;
        const height = chatRef.current.scrollHeight;
        return Math.ceil((height / MIN_MESSAGE_HEIGHT) * 1.2);
    };

    const initChat = useCallback(async (users: ScAddr[]) => {
        const chatNode = await dialogAgent(users);
        if (!chatNode) return;

        const { messages, shouldEnd } = await searchChatMessages(chatNode, minNumberMessages());
        if (!messages) return;
        setShouldEnd(shouldEnd);
        setMessages(messages);
        setChatNode(chatNode);
    }, []);

    useEffect(() => {
        subscribeToMessage();
    }, [subscribeToMessage]);

    const onFetching = useCallback(async () => {
        const lastMessageScAddr = messages[0].addr;
        if (shouldEnd || !chatNode) return;

        const { messages: newMessages, shouldEnd: localShouldEnd } = await searchChatMessages(
            chatNode,
            minNumberMessages(),
            lastMessageScAddr,
        );
        setMessages((prev) => [...newMessages, ...prev]);

        return localShouldEnd;
    }, [chatNode, shouldEnd, messages]);

    const sendMessage = useCallback(
        async (user: ScAddr, text: string) => {
            const linkAddr = await generateLinkText(text);
            if (!linkAddr || !chatNode) return;
            const date = new Date();
            const message = {
                id: linkAddr.value,
                text,
                author: user,
                time: `${date.getHours().toString().padStart(2, '0')}:${date.getMinutes().toString().padStart(2, '0')}`,
                date: `${date.getFullYear()}.${(date.getMonth() + 1).toString().padStart(2, '0')}.${date
                    .getDate()
                    .toString()
                    .padStart(2, '0')}`,
                addr: chatNode,
                isLoading: true,
            };
            setMessages((prev) => [...prev, message]);
            await newMessageAgent(chatNode, user, linkAddr);

            setIsAgentAnswer(false);
        },
        [chatNode],
    );

    return { initChat, sendMessage, isAgentAnswer, onFetching, messages, chatRef };
};
