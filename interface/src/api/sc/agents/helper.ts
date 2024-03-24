import { ScAddr, ScTemplate, ScType } from 'ts-sc-client';
import { client } from '@api/sc/client';
import { ScConstruction, ScLinkContent, ScLinkContentType, ScEventParams, ScEventType } from 'ts-sc-client';
import { makeAgent } from '@api/sc/agents/makeAgent';
import { createLinkText } from './newMessageAgent';
import React from "react";

const nrelSystemIdentifier = 'nrel_system_identifier';
const nrelAuthors = 'nrel_authors';
const nrelScTextTranslation = 'nrel_sc_text_translation';
const question = 'question';
const actionCreateQuestionClassAndPhraseTemplate = 'action_create_question_class_and_phrase_template';
const actionCreateClass = 'action_create_class';
const rrel1 = 'rrel_1';
const rrel2 = 'rrel_2';
const conceptTextFile = 'concept_text_file';
const conceptMessage = 'concept_message';
const conceptDialogue = 'concept_dialogue';
const langRu = 'lang_ru';
const message = '_message';
const myself = 'myself';

const baseKeynodes = [
    { id: nrelSystemIdentifier, type: ScType.NodeConstNoRole},
    { id: nrelAuthors, type: ScType.NodeConstNoRole},
    { id: nrelScTextTranslation, type: ScType.NodeConstNoRole},
    { id: question, type: ScType.NodeConstClass },
    { id: actionCreateQuestionClassAndPhraseTemplate, type: ScType.NodeConstClass },
    { id: rrel1, type: ScType.NodeConstRole },
    { id: rrel2, type: ScType.NodeConstRole },
    { id: conceptTextFile, type: ScType.NodeConstClass },
    { id: conceptMessage, type: ScType.NodeConstClass },
    { id: conceptDialogue, type: ScType.NodeConstClass },
    { id: langRu, type: ScType.NodeConstClass },
    { id: message, type: ScType.NodeVar},
    { id: actionCreateClass, type: ScType.NodeConstClass},
    { id: myself, type: ScType.NodeConst}
];

const setSystemIdtf = async (addr: ScAddr, systemIdtf: string) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const template = new ScTemplate();
    const linkAlias = "_link";

    const sysIdtfLinkConstruction = new ScConstruction();
    sysIdtfLinkConstruction.createLink(
        ScType.LinkConst,
        new ScLinkContent(systemIdtf, ScLinkContentType.String)
    );
    const [sysIdtfLinkNode] = await client.createElements(sysIdtfLinkConstruction);

    template.tripleWithRelation(
      addr,
      ScType.EdgeDCommonVar,
      sysIdtfLinkNode,
      ScType.EdgeAccessVarPosPerm,
      keynodes[nrelSystemIdentifier]
    );
    const result = await client.templateGenerate(template, {});
};

const createReply = async (message: string) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const messageLinkConstruction = new ScConstruction();
    messageLinkConstruction.createLink(
        ScType.LinkConst,
        new ScLinkContent(message, ScLinkContentType.String)
    );
    const [messageLinkNode] = await client.createElements(messageLinkConstruction);
    
    const tupleNode = '_tuple_node';
    const messageNode = '_message_node';
    const requiredNodes = new ScConstruction();
    requiredNodes.createNode(ScType.NodeConst, tupleNode);
    requiredNodes.createNode(ScType.NodeConst, messageNode);
    
    const nodes = await client.createElements(requiredNodes);
    const target = new ScAddr(393287);
    const template = new ScTemplate();
    template.triple(
        nodes[0],
        ScType.EdgeAccessVarPosPerm,
        messageLinkNode
    );
    template.tripleWithRelation(
        nodes[0],
        ScType.EdgeDCommonVar,
        nodes[1],
        ScType.EdgeAccessVarPosPerm,
        keynodes[nrelScTextTranslation]
    );
    template.tripleWithRelation(
        nodes[1],
        ScType.EdgeDCommonVar,
        keynodes[myself],
        ScType.EdgeAccessVarPosPerm,
        keynodes[nrelAuthors]
    );
    template.triple(
        keynodes[conceptMessage],
        ScType.EdgeAccessVarPosPerm,
        nodes[1]
    );
    template.triple(
        target,
        ScType.EdgeAccessVarPosPerm,
        nodes[1]
    );
    template.triple(
        keynodes[langRu],
        ScType.EdgeAccessVarPosPerm,
        messageLinkNode
    );

    const res = await client.templateGenerate(template, {});
    
}

export const handleSave = async (
    phraseSystemIdentifierRef: React.RefObject<HTMLInputElement>,
    phraseRussianIdentifierRef: React.RefObject<HTMLInputElement>,
    form : string,
    chipsValues: string[]
) => {
        const inputValues = {
            phraseSystemIdentifier: phraseSystemIdentifierRef.current?.value || '',
            phraseRussianIdentifier: phraseRussianIdentifierRef.current?.value || '',
            };

        const phrases = chipsValues.join(', ');

        const result : string = form + '\n' + Object.values(inputValues).join('\n') + '\n' + phrases;

        const resultLinkAddr = await createLinkText(result);
        
        if (resultLinkAddr !== null) {
            await createQuestionClassAndPhraseTemplateAgent(resultLinkAddr);
        }
};

export const userClose = async () => {
    const result : string = "User close";

    const resultLinkAddr = await createLinkText(result);
    
    if (resultLinkAddr !== null) {
        await createQuestionClassAndPhraseTemplateAgent(resultLinkAddr);
    }    
}

const describeAgent = async (
    keynodes: Record<string, ScAddr>,
    linkAddr: ScAddr,
    action: string
) => {
    const actionNodeAlias = '_action_node';

    const template = new ScTemplate();

    template.triple(keynodes[question], ScType.EdgeAccessVarPosPerm, [ScType.NodeVar, actionNodeAlias]);
    template.triple(keynodes[action], ScType.EdgeAccessVarPosPerm, actionNodeAlias);

    template.tripleWithRelation(
        actionNodeAlias,
        ScType.EdgeAccessVarPosPerm,
        keynodes[message],
        ScType.EdgeAccessVarPosPerm,
        keynodes[rrel1],
    );
    template.tripleWithRelation(
        actionNodeAlias,
        ScType.EdgeAccessVarPosPerm,
        linkAddr,
        ScType.EdgeAccessVarPosPerm,
        keynodes[rrel2],
    );
    template.triple(keynodes[conceptTextFile], ScType.EdgeAccessVarPosPerm, linkAddr);
    template.triple(keynodes[langRu], ScType.EdgeAccessVarPosPerm, linkAddr);

    return [template, actionNodeAlias] as const;
};

const createQuestionClassAndPhraseTemplateAgent = async (linkAddr: ScAddr) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);

    const [template, userActionNodeAlias] = await describeAgent(keynodes, linkAddr, actionCreateQuestionClassAndPhraseTemplate);

    await makeAgent(template, userActionNodeAlias);
};

export const createPopupCheck = async (
    setCreatePopup
)  => {
    const concept_popup = 'concept_popup_component_for_creating_message_class_and_phrase_template';

    const baseKeynodes = [
        { id: concept_popup, type: ScType.NodeConstClass},
    ];

    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const eventParams = new ScEventParams(keynodes[concept_popup], ScEventType.AddOutgoingEdge, () => {setCreatePopup(true)});
    await client.eventsCreate([eventParams])
}