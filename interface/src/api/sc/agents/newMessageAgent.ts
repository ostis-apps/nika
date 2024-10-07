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
const answer = 'nrel_answer';

const baseKeynodes = [
    { id: action, type: ScType.NodeConstClass },
    { id: actionInitiated, type: ScType.NodeConstClass },
    { id: actionReplyToMessage, type: ScType.NodeConstClass },
    { id: rrel1, type: ScType.NodeConstRole },
    { id: rrel2, type: ScType.NodeConstRole },
    { id: nrelAuthors, type: ScType.NodeConstNoRole },
    { id: conceptTextFile, type: ScType.NodeConstClass },
    { id: langEn, type: ScType.NodeConstClass },
    { id: langRu, type: ScType.NodeConstClass },
    { id: actionFinished, type: ScType.NodeConstClass },
    { id: answer, type: ScType.NodeConstNoRole },
];

export const createLinkText = async (messageText: string) => {
    const constructionLink = new ScConstruction();
    constructionLink.createLink(ScType.LinkConst, new ScLinkContent(messageText, ScLinkContentType.String));
    const resultLinkNode = await client.createElements(constructionLink);
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

    template.triple(keynodes[action], ScType.EdgeAccessVarPosPerm, [ScType.NodeVar, actionNodeAlias]);
    template.triple(keynodes[actionReplyToMessage], ScType.EdgeAccessVarPosPerm, actionNodeAlias);

    template.tripleWithRelation(
        actionNodeAlias,
        ScType.EdgeAccessVarPosPerm,
        linkAddr,
        ScType.EdgeAccessVarPosPerm,
        keynodes[rrel1],
    );
    template.tripleWithRelation(
        actionNodeAlias,
        ScType.EdgeAccessVarPosPerm,
        chatNode,
        ScType.EdgeAccessVarPosPerm,
        keynodes[rrel2],
    );
    template.tripleWithRelation(
        actionNodeAlias,
        ScType.EdgeDCommonVar,
        author,
        ScType.EdgeAccessVarPosPerm,
        keynodes[nrelAuthors],
    );
    template.triple(keynodes[conceptTextFile], ScType.EdgeAccessVarPosPerm, linkAddr);
    template.triple(keynodes[langRu], ScType.EdgeAccessVarPosPerm, linkAddr);

    return [template, actionNodeAlias] as const;
};

const findNewMessageNode = async (circuitAddr: ScAddr) => {
    const message = '_message';
    const template = new ScTemplate();

    template.triple(circuitAddr, ScType.EdgeAccessVarPosPerm, [ScType.NodeVar, message]);
    const resultMessageNode = await client.templateSearch(template);

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
