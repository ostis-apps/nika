import { ScAddr, ScTemplate, ScType } from 'ts-sc-client';
import { client } from '@api/sc/client';
import { ScConstruction, ScLinkContent, ScLinkContentType, ScEventParams, ScEventType } from 'ts-sc-client';
import { makeAgent } from '@api/sc/agents/makeAgent';
import { createLinkText } from './newMessageAgent';
import React from "react";

const nrelSystemIdentifier = 'nrel_system_identifier';
const question = 'question';
const actionCreateMessageClassAndPhraseTemplate = 'action_create_message_class_and_phrase_template';
const actionCreateClassInstance = 'action_create_class_instance';
const actionCreateClass = 'action_create_class';
const actionCreateRelation = 'action_create_relation';
const rrel1 = 'rrel_1';
const rrel2 = 'rrel_2';
const conceptTextFile = 'concept_text_file';
const langRu = 'lang_ru';
const message = '_message';

const baseKeynodes = [
    { id: nrelSystemIdentifier, type: ScType.NodeConstNoRole},
    { id: question, type: ScType.NodeConstClass },
    { id: actionCreateMessageClassAndPhraseTemplate, type: ScType.NodeConstClass },
    { id: actionCreateClassInstance, type: ScType.NodeConstClass },
    { id: actionCreateClass, type: ScType.NodeConstClass },
    { id: actionCreateRelation, type: ScType.NodeConstClass },
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

export const handleSaveClassInstance = async (
    classInstanceSystemIdentifierRef: React.RefObject<HTMLInputElement>,
    classInstanceRussianIdentifierRef: React.RefObject<HTMLInputElement>,
    classInstanceNoteRef: React.RefObject<HTMLInputElement>,
    classInstanceClass: string,
    
) => {
        const inputValues = {
            classInstanceSystemIdentifier: classInstanceSystemIdentifierRef.current?.value || '',
            classInstanceRussianIdentifier: classInstanceRussianIdentifierRef.current?.value || '',
            classInstanceNote: classInstanceNoteRef.current?.value || '',
            classInstanceClass: classInstanceClass
            };

        const result : string = Object.values(inputValues).join('\n');
        const resultLinkAddr = await createLinkText(result);
        
        if (resultLinkAddr !== null) {
            await createAgent(resultLinkAddr, actionCreateClassInstance);
        }
};

export const handleSaveClassInstanceWithRelations = async (
    firstForm: string[],
    form: {entity:string; relation:string;}[]
) => {
        const result : string = firstForm.join('\n') + "\n" + form.map(item => `${item.entity} - ${item.relation}`).join(', ');
        const resultLinkAddr = await createLinkText(result);
        
        if (resultLinkAddr !== null) {
            await createAgent(resultLinkAddr, actionCreateClassInstance);
        }
};

export const handleRelationInstance = async (
    relationSystemIdentifierRef: React.RefObject<HTMLInputElement>,
    relationRussianIdentifierRef: React.RefObject<HTMLInputElement>,
    relationNoteRef: React.RefObject<HTMLInputElement>,
    relationFirstDomain: string,
    relationSecondDomain: string,
    isBinary: boolean,
    isOriented: boolean,
    isAntireflexive: boolean,
    isAsymmetric: boolean,
    isAntitransitive: boolean,
) => {
        const inputValues = {
            SystemIdentifier: relationSystemIdentifierRef.current?.value || '',
            RussianIdentifier: relationRussianIdentifierRef.current?.value || '',
            Note: relationNoteRef.current?.value || '',
            FirstDomain: relationFirstDomain,
            SecondDomain: relationSecondDomain
            };
        const relationClassificationList = getRelationClassificationList(isBinary, isOriented, isAntireflexive, isAsymmetric, isAntitransitive);
        const result : string = Object.values(inputValues).join('\n') + '\n' + relationClassificationList.join(', ');
        const resultLinkAddr = await createLinkText(result);
        console.log(result);
        if (resultLinkAddr !== null) {
            await createAgent(resultLinkAddr, actionCreateRelation);
        }
};

export const handleSaveToCreateClass = async (
    classSystemIdentifierRef: React.RefObject<HTMLInputElement>,
    classRussianIdentifierRef: React.RefObject<HTMLInputElement>,
    classNoteRef: React.RefObject<HTMLInputElement>,
    classSuperClass: string,
    chipsValues: string[]
) => {
        const inputValues = {
            phraseSystemIdentifier: classSystemIdentifierRef.current?.value || '',
            phraseRussianIdentifier: classRussianIdentifierRef.current?.value || '',
            classNote: classNoteRef.current?.value || '',
            classSuperClass: classSuperClass,
            };

        console.log(inputValues);

        const phrases = chipsValues.join(', ');

        const result : string = Object.values(inputValues).join('\n') + '\n' + phrases;

        const resultLinkAddr = await createLinkText(result);
        
        if (resultLinkAddr !== null) {
            await createAgent(resultLinkAddr, actionCreateClass);
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
export const getRelationClassificationList = (binary: boolean, oriented: boolean, antireflexive: boolean, asymmetric: boolean, antitransitive: boolean) => {
    const classificationList : string[] = [];

    if (binary)
    { classificationList.push("binary_relation")}

    if (oriented)
    { classificationList.push("oriented_relation")}

    if (antireflexive)
    { classificationList.push("antireflexive_relation")}

    if (asymmetric)
    { classificationList.push("asymmetric_relation")}

    if (antitransitive)
    { classificationList.push("antitransitive_relation")}

    return classificationList
}

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

  

