import { ScAddr, ScTemplate, ScType } from 'ts-sc-client';
import { client } from '@api/sc/client';
import { ScConstruction, ScLinkContent, ScLinkContentType, ScEventParams, ScEventType } from 'ts-sc-client';
import { makeAgent } from '@api/sc/agents/makeAgent';
import { createLinkText } from './newMessageAgent';
import React from "react";

const nrelSystemIdentifier = 'nrel_system_identifier';
const question = 'question';
const actionCreateMessageClassAndPhraseTemplate = 'action_create_message_class_and_phrase_template';
const rrel1 = 'rrel_1';
const rrel2 = 'rrel_2';
const conceptTextFile = 'concept_text_file';
const langRu = 'lang_ru';
const message = '_message';

const baseKeynodes = [
    { id: nrelSystemIdentifier, type: ScType.NodeConstNoRole},
    { id: question, type: ScType.NodeConstClass },
    { id: actionCreateMessageClassAndPhraseTemplate, type: ScType.NodeConstClass },
    { id: rrel1, type: ScType.NodeConstRole },
    { id: rrel2, type: ScType.NodeConstRole },
    { id: conceptTextFile, type: ScType.NodeConstClass },
    { id: langRu, type: ScType.NodeConstClass },
    { id: message, type: ScType.NodeVar},
];
//Функции для вызова агентов по нажатию кнопки
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
            await createAgent(resultLinkAddr, actionCreateMessageClassAndPhraseTemplate);
        }
};

export const userClose = async () => {
    const result : string = "User close";

    const resultLinkAddr = await createLinkText(result);
    
    if (resultLinkAddr !== null) {
        await createAgent(resultLinkAddr, actionCreateMessageClassAndPhraseTemplate);
    }    
}
//Функция для открытия компонентов
export const createPopupCheck = async (
    setCreatePopup,
    conceptName: string
)  => {

    const baseKeynodes = [
        { id: conceptName, type: ScType.NodeConstClass},
    ];

    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const eventParams = new ScEventParams(keynodes[conceptName], ScEventType.AddOutgoingEdge, () => {setCreatePopup(true)});
    await client.eventsCreate([eventParams])
}
//Функции добавления агента в программу обработки
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

const createAgent = async (linkAddr: ScAddr, action: string) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const [template, userActionNodeAlias] = await describeAgent(keynodes, linkAddr, action);
    await makeAgent(template, userActionNodeAlias);
}

//Вспомагательные функции
export const findAnyInKb = async (setList: (options: { label: string; value: string }[]) => void, param: string): Promise<void> => {
    const list = await client.getLinksContentsByContentSubstrings([param]);
    const options = list[0]
      .filter((word) => word.startsWith(param))
      .map((word, index) => ({
        label: word,
        value: (index + 1).toString(),
      }));
    setList(options);
  };

  export const findRelationsInKb = async (setList: (options: { label: string; value: string }[]) => void): Promise<void> => {
    const list = await client.getLinksContentsByContentSubstrings(["nrel_", "rrel_"]);
    const options = list[0]
      .map((word, index) => ({
        label: word,
        value: (index + 1).toString(),
      }));
    setList(options);
  };

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

  

