import { ScAddr, ScTemplate, ScType } from 'ts-sc-client';
import { client } from '@api/sc/client';
import { ScConstruction, ScLinkContent, ScLinkContentType, ScEventParams, ScEventType } from 'ts-sc-client';

const nrelSystemIdentifier = 'nrel_system_identifier';

const baseKeynodes = [
    { id: nrelSystemIdentifier, type: ScType.NodeConstNoRole},
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


export const checkToCreatePopup = async (
    createPopup : boolean
) => {
    const concept_popup = 'concept_popup';
    const create_phrase_template_popup = '_create_phrase_template_popup';

    const baseKeynodes = [
        { id: concept_popup, type: ScType.NodeConstClass},
        { id: create_phrase_template_popup, type: ScType.NodeVar},
    ];

    const keynodes = client.resolveKeynodes(baseKeynodes);
    console.log("hello");
    const eventParams = new ScEventParams(keynodes[concept_popup], ScEventType.AddOutgoingEdge, ()=>{createPopup = true; console.log("Hello");});
    console.log("world");
    await client.eventsCreate([eventParams]);
}

export const handleSave = async (
    systemIdentifierRef: React.RefObject<HTMLInputElement>,
    russianIdentifierRef: React.RefObject<HTMLInputElement>,
    englishIdentifierRef: React.RefObject<HTMLInputElement>,
    chipsValues: string[]
) => {
        const inputValues = {
            systemIdentifier: systemIdentifierRef.current?.value || '',
            russianIdentifier: russianIdentifierRef.current?.value || '',
            englishIdentifier: englishIdentifierRef.current?.value || '',
            };

        const phrases = chipsValues.join(', ');

        const inputs = [
            'System Identifier: ' + inputValues['systemIdentifier'],
            'Russian Identifier: ' + inputValues['russianIdentifier'],
            'English Identifier: ' + inputValues['englishIdentifier'],
            'Answer Templates: ' + phrases
        ];

        const inputsConstruction = new ScConstruction();

        for(let inputText of inputs)
        {
            inputsConstruction.createLink(
                ScType.LinkConst,
                new ScLinkContent(inputText, ScLinkContentType.String)
            );
        }

        const inputsLinks = await client.createElements(inputsConstruction);

        const success = new ScConstruction();

        success.createNode(ScType.NodeConstClass, 'success');
        
        const [statusSuccess] = await client.createElements(success);

        setSystemIdtf(statusSuccess, 'Success');

        console.log(inputValues);
};

