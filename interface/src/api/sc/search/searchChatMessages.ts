import { ScAddr, ScTemplate, ScType } from 'ts-sc-client';
import { client } from "@api";

const rrel1 = 'rrel_1';
const nrelAuth = 'nrel_authors';
const nrelScTextTranslation = 'nrel_sc_text_translation';
const nrelMessageSequence = 'nrel_message_sequence';
const rrelLast = 'rrel_last';

const baseKeynodes = [
    { id: rrel1, type: ScType.ConstNodeRole },
    { id: nrelAuth, type: ScType.ConstNodeNonRole },
    { id: nrelScTextTranslation, type: ScType.ConstNodeNonRole },
    { id: nrelMessageSequence, type: ScType.ConstNodeNonRole },
    { id: rrelLast, type: ScType.ConstNodeRole },
];

interface IMessage {
    addr: ScAddr;
    text: string | number;
    id: number;
    time: string | number;
    date: string;
    author: ScAddr;
}

const findLastMessageNode = async (chatNode: ScAddr, keynodes: Record<string, ScAddr>) => {
    const messagesClassNodeAlias = '_messages_node';
    const template = new ScTemplate();

    template.quintuple(
        chatNode,
        ScType.VarPermPosArc,
        [ScType.VarNode, messagesClassNodeAlias],
        ScType.VarPermPosArc,
        keynodes[rrelLast],
    );
    const resultMessagesClassNode = await client.searchByTemplate(template);
    if (resultMessagesClassNode.length) {
        return resultMessagesClassNode[0].get(messagesClassNodeAlias);
    }
    return null;
};
const findCurrentEdge = async (chatNode: ScAddr, messageNode: ScAddr) => {
    const currentEdgeAlias = '_current_edge';

    const template = new ScTemplate();
    template.triple(chatNode, [ScType.VarPermPosArc, currentEdgeAlias], messageNode);
    const resultLastMessageEdge = await client.searchByTemplate(template);

    if (resultLastMessageEdge.length) {
        return resultLastMessageEdge[0].get(currentEdgeAlias);
    }
    return null;
};

const findPreviousMessageNode = async (chatNode: ScAddr, currentEdge: ScAddr) => {
    const previousMessageClassAlias = '_privious_message_class';

    const templatePreviousMessageClass = new ScTemplate();
    templatePreviousMessageClass.triple(chatNode, currentEdge, [ScType.VarNode, previousMessageClassAlias]);

    const resultNextMessageClassEdge = await client.searchByTemplate(templatePreviousMessageClass);
    if (resultNextMessageClassEdge.length) {
        return resultNextMessageClassEdge[0].get(previousMessageClassAlias);
    }
    return null;
};
const findPreviousEdge = async (chatNode: ScAddr, keynodes: Record<string, ScAddr>, currrentMessageNode: ScAddr) => {
    const nextMessageEdgeAlias = '_previous_message_edge';
    const currentMessageEdgeAlias = '_previous_message_example_edge';

    const templateNextEdge = new ScTemplate();

    templateNextEdge.triple(chatNode, [ScType.VarPermPosArc, nextMessageEdgeAlias], ScType.VarNode);
    templateNextEdge.triple(chatNode, [ScType.VarPermPosArc, currentMessageEdgeAlias], currrentMessageNode);
    templateNextEdge.quintuple(
        nextMessageEdgeAlias,
        ScType.VarCommonArc,
        currentMessageEdgeAlias,
        ScType.VarPermPosArc,
        keynodes[nrelMessageSequence],
    );

    const resultNextMessageClassEdge = await client.searchByTemplate(templateNextEdge);
    if (resultNextMessageClassEdge.length) {
        return resultNextMessageClassEdge[0].get(nextMessageEdgeAlias);
    }
    return null;
};

export const getInfoMessage = async (messagesNode: ScAddr, keynodes: Record<string, ScAddr>) => {
    const messageNodeAlias = '_message_node';
    const textNodeAlias = '_text_node';
    const authorNodeAlias = '_author_node';

    const template = new ScTemplate();

    template.quintuple(
        [ScType.VarNode, messageNodeAlias],
        ScType.VarCommonArc,
        messagesNode,
        ScType.VarPermPosArc,
        keynodes[nrelScTextTranslation],
    );
    template.triple(messageNodeAlias, ScType.VarPermPosArc, [ScType.VarNodeLink, textNodeAlias]);
    template.quintuple(
        messagesNode,
        ScType.VarCommonArc,
        [ScType.VarNode, authorNodeAlias],
        ScType.VarPermPosArc,
        keynodes[nrelAuth],
    );

    const result = await client.searchByTemplate(template);

    if (!result.length) return;

    const linkNode = result[0].get(textNodeAlias);

    const resultText = await client.getLinkContents([linkNode]);
    const text = resultText[0].data;

    const author = result[0].get(authorNodeAlias);

    const date = new Date();

    return {
        id: linkNode.value,
        text,
        time: `${date.getHours().toString().padStart(2, '0')}:${date.getMinutes().toString().padStart(2, '0')}`,
        date: `${date.getFullYear()}.${(date.getMonth() + 1).toString().padStart(2, '0')}.${date
            .getDate()
            .toString()
            .padStart(2, '0')}`,
        author: author,
        addr: messagesNode };
};

const checkRrel1 = async (keynodes: Record<string, ScAddr>, chatNode: ScAddr, messageNode: ScAddr) => {
    const template = new ScTemplate();
    template.quintuple(
        chatNode,
        ScType.VarPermPosArc,
        messageNode,
        ScType.VarPermPosArc,
        keynodes[rrel1],
    );
    const result = await client.searchByTemplate(template);

    return !!result.length;
};

const getFirstMessages = async (
    chatNode: ScAddr,
    keynodes: Record<string, ScAddr>,
    amount: number,
    lastMessageScAddr?: ScAddr,
) => {
    const messages: IMessage[] = [];
    const defaultReturn = { messages: [], shouldEnd: false };
    let count = 0;
    let currentMessagesNode = lastMessageScAddr || (await findLastMessageNode(chatNode, keynodes));

    if (!currentMessagesNode) return defaultReturn;

    let currentEdge = await findCurrentEdge(chatNode, currentMessagesNode);

    if (!currentEdge) return defaultReturn;

    const lastMessage = await getInfoMessage(currentMessagesNode, keynodes);
    if (!lastMessage) return defaultReturn;
    if (!lastMessageScAddr) messages.push(lastMessage);

    while (count < amount) {
        count++;
        if (!currentMessagesNode) break;
        currentEdge = await findPreviousEdge(chatNode, keynodes, currentMessagesNode);

        if (!currentEdge) break;

        currentMessagesNode = await findPreviousMessageNode(chatNode, currentEdge);

        if (!currentMessagesNode) break;

        const message = await getInfoMessage(currentMessagesNode, keynodes);
        if (!message) break;
        messages.unshift(message);
    }
    const shouldEnd = messages[0] ? await checkRrel1(keynodes, chatNode, messages[0].addr) : true;

    return { messages, shouldEnd };
};

export const searchChatMessages = async (chatNode: ScAddr, amount: number, lastMessageScAddr?: ScAddr) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    return await getFirstMessages(chatNode, keynodes, amount, lastMessageScAddr);
};
