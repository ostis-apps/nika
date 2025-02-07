import { client } from '@api/sc';
import { ScAddr, ScConstruction, ScLinkContent, ScTemplate, ScType, ScLinkContentType } from 'ts-sc-client';
import { makeAgent } from '@api/sc/agents/makeAgent';

const action = 'action';
const actionInitiated = 'action_initiated';
const actionReplyToMessage = 'action_reply_to_message';
const rrel1 = 'rrel_1';
const rrel2 = 'rrel_2';
const nrelAuthors = 'nrel_authors';
const conceptTextFile = 'concept_text_file';
const langEn = 'lang_en';
const langRu = 'lang_ru';
const actionFinished = 'action_finished';
const result = 'nrel_result';

const baseKeynodes = [
    { id: action, type: ScType.ConstNodeClass },
    { id: actionInitiated, type: ScType.ConstNodeClass },
    { id: actionReplyToMessage, type: ScType.ConstNodeClass },
    { id: rrel1, type: ScType.ConstNodeRole },
    { id: rrel2, type: ScType.ConstNodeRole },
    { id: nrelAuthors, type: ScType.ConstNodeNonRole },
    { id: conceptTextFile, type: ScType.ConstNodeClass },
    { id: langEn, type: ScType.ConstNodeClass },
    { id: langRu, type: ScType.ConstNodeClass },
    { id: actionFinished, type: ScType.ConstNodeClass },
    { id: result, type: ScType.ConstNodeNonRole },
];

export const generateLinkText = async (messageText: string) => {
    const constructionLink = new ScConstruction();
    constructionLink.generateLink(ScType.ConstNodeLink, new ScLinkContent(messageText, ScLinkContentType.String));
    const resultLinkNode = await client.generateElements(constructionLink);
    if (resultLinkNode.length) {
        return resultLinkNode[0];
    }
    return null;
};

const describeAgent = async (
    chatNode: ScAddr,
    author: ScAddr | string,
    keynodes: Record<string, ScAddr>,
    linkAddr: ScAddr,
) => {
    const actionNodeAlias = '_action_node';

    const template = new ScTemplate();

    template.triple(keynodes[action], ScType.VarPermPosArc, [ScType.VarNode, actionNodeAlias]);
    template.triple(keynodes[actionReplyToMessage], ScType.VarPermPosArc, actionNodeAlias);

    template.quintuple(
        actionNodeAlias,
        ScType.VarPermPosArc,
        linkAddr,
        ScType.VarPermPosArc,
        keynodes[rrel1],
    );
    template.quintuple(
        actionNodeAlias,
        ScType.VarPermPosArc,
        chatNode,
        ScType.VarPermPosArc,
        keynodes[rrel2],
    );
    template.quintuple(
        actionNodeAlias,
        ScType.VarCommonArc,
        author,
        ScType.VarPermPosArc,
        keynodes[nrelAuthors],
    );
    template.triple(keynodes[conceptTextFile], ScType.VarPermPosArc, linkAddr);
    template.triple(keynodes[langRu], ScType.VarPermPosArc, linkAddr);

    return [template, actionNodeAlias] as const;
};

const findNewMessageNode = async (circuitAddr: ScAddr) => {
    const message = '_message';
    const template = new ScTemplate();

    template.triple(circuitAddr, ScType.VarPermPosArc, [ScType.VarNode, message]);
    const resultMessageNode = await client.searchByTemplate(template);

    if (resultMessageNode.length) {
        return resultMessageNode[0].get(message);
    }
    return null;
};

export const newMessageAgent = async (chatNode: ScAddr, author: ScAddr | string, linkAddr: ScAddr) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const [template, userActionNodeAlias] = await describeAgent(chatNode, author, keynodes, linkAddr);

    const circuitAddr = await makeAgent(template, userActionNodeAlias);

    if (!circuitAddr) return null;

    return await findNewMessageNode(circuitAddr);
};
