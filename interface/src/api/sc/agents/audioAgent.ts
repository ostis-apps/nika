import { client } from '@api/sc';
import { ScAddr, ScConstruction, ScLinkContent, ScTemplate, ScType, ScLinkContentType } from 'ts-sc-client';
import { makeAgent } from '@api/sc/agents/makeAgent';
declare var responsiveVoice: any;
const nrelScTextTranslation = 'nrel_sc_text_translation';
const langEn = 'lang_en';
const langRu = 'lang_ru';
const action = 'action';
const actionInitiated = 'action_initiated';
const actionText2Speech = 'action_text_to_speech';
const rrel1 = 'rrel_1';
const actionFinished = 'action_finished';

const baseKeynodes = [
    { id: action, type: ScType.NodeConstClass },
    { id: actionInitiated, type: ScType.NodeConstClass },
    { id: actionText2Speech, type: ScType.NodeConstClass },
    { id: rrel1, type: ScType.NodeConstRole },
    { id: actionFinished, type: ScType.NodeConstClass },
    { id: nrelScTextTranslation, type: ScType.NodeConstNoRole},
    { id: langEn, type: ScType.NodeConstClass },
    { id: langRu, type: ScType.NodeConstClass },
];

const describeAgent = async (
    message: ScAddr,
    keynodes: Record<string, ScAddr>,
) => {
    const actionNodeAlias = '_action_node';
    const template = new ScTemplate();
    template.triple(keynodes[action], ScType.EdgeAccessVarPosPerm, [ScType.NodeVar, actionNodeAlias]);
    template.triple(keynodes[actionText2Speech], ScType.EdgeAccessVarPosPerm, actionNodeAlias);
    template.tripleWithRelation(
        actionNodeAlias,
        ScType.EdgeAccessVarPosPerm,
        message,
        ScType.EdgeAccessVarPosPerm,
        keynodes[rrel1],
    );
    const generationResult = await client.templateGenerate(template, {});
    if (generationResult && generationResult.size > 0) {
        let actionNode = generationResult.get(actionNodeAlias);
        return actionNode;
    }
    return null;
};

export const callText2SpeechAgent = async (message: ScAddr) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const actionNode = await describeAgent(message, keynodes);
    if (actionNode == null) return;
    const construction = new ScConstruction();
    construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes[actionInitiated], actionNode);
    client.createElements(construction);
};

export const check = async(action: ScAddr,keynodes: Record<string, ScAddr>) => {
    const checkTemplate = new ScTemplate();
    checkTemplate.triple(
        keynodes[actionText2Speech],
        ScType.EdgeAccessVarPosPerm,
        action,
    );
    const checkResult = await client.templateSearch(checkTemplate);
    if (checkResult.length==0)
        {
            console.log("No text to speech action");
            return false;

        }
        console.log("text to speech action");
    return true;
}

export const getLinkNode= async(action: ScAddr,keynodes: Record<string, ScAddr>) => {
    const arg = '_arg';
    const answer_construction = '_answer_construction';
    const translation='_translation'
    const link="_link";
    const template = new ScTemplate();
    template.triple(
        keynodes[actionText2Speech],
        ScType.EdgeAccessVarPosPerm,
        action,
    )
    template.tripleWithRelation(
        action,
        ScType.EdgeAccessVarPosPerm,
        [ScType.NodeVar, arg],
        ScType.EdgeAccessVarPosPerm,
        keynodes[rrel1],
    );
    template.triple(
        arg,
        ScType.EdgeAccessVarPosTemp,
        [ScType.NodeVar, answer_construction],
    )
    template.tripleWithRelation(
        [ScType.NodeVar, translation],
        ScType.EdgeDCommonVar,
        answer_construction,
        ScType.EdgeAccessVarPosPerm,
        keynodes[nrelScTextTranslation],
    )
    template.triple(translation,ScType.EdgeAccessVarPosPerm,[ScType.LinkVar, link]);
    const result=await client.templateSearch(template);
    if (!result.length) {
        console.log("Construction is undefined")
        return false;
    }
    const linkNode = result[0].get(link);
    return linkNode;
}

export const getText= async(linkNode: ScAddr,keynodes: Record<string, ScAddr>) => {
    const resultText = await client.getLinkContents([linkNode]);
    const text = resultText[0].data as string;
    return text.replace(/<[^>]*>/g, '').replace(/\.{3}|[.;"]/g, '');}


export const getLang= async(linkNode: ScAddr,keynodes: Record<string, ScAddr>) => {
    const enTemplate=new ScTemplate();
    enTemplate.triple(keynodes[langEn],ScType.EdgeAccessVarPosPerm,linkNode);
    const enResult=await client.templateSearch(enTemplate);
    const ruTemplate=new ScTemplate();
    ruTemplate.triple(keynodes[langRu],ScType.EdgeAccessVarPosPerm,linkNode);
    const ruResult=await client.templateSearch(ruTemplate);
    if (ruResult.length!=0)
        {
            return "Russian Female" as string;
        }
    else if (enResult.length!=0) 
        {
            return "UK English Female" as string;                    
        }
    else {
        console.log("Language is undefined")
        return false;
    }
}

export const speak= async(text: string,lang: string) => {
    if(responsiveVoice.voiceSupport()) {
        responsiveVoice.speak(text,lang);  
    } else {
        console.log("Your browser doesn't support TTS");
    }     
}


export const audioAgent = async(subscribedAddr: ScAddr, foundEgde: ScAddr, action: ScAddr) => {
    const actionText2Speech = "action_text_to_speech";
    const nrelScTextTranslation = 'nrel_sc_text_translation';
    const langEn = 'lang_en';
    const langRu = 'lang_ru';
    const rrel1 = 'rrel_1';
    const actionFinished = 'action_finished';
    const baseKeynodes = [
        { id: actionText2Speech, type: ScType.NodeConstNoRole },
        { id: rrel1, type: ScType.NodeConstRole },
        { id: nrelScTextTranslation, type: ScType.NodeConstNoRole},
        { id: langEn, type: ScType.NodeConstClass },
        { id: langRu, type: ScType.NodeConstClass },
        { id: actionFinished, type: ScType.NodeConstClass },
    ];   
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    if(await check(action,keynodes)===false) 
    {
        return;
    }
    const linkNode=await getLinkNode(action,keynodes);
    if (linkNode===false) {
        const construction = new ScConstruction();
        construction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[actionFinished], action);
        client.createElements(construction); 
        return;
       }
    const lang=await getLang(linkNode,keynodes);
    const text=await getText(linkNode,keynodes);
    if (lang===false) {
        const construction = new ScConstruction();
        construction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[actionFinished], action);
        client.createElements(construction); 
        return;
       }
    console.log(text);
    speak(text,lang);
    const construction = new ScConstruction();
    construction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[actionFinished], action);
    client.createElements(construction);
    return;
};










