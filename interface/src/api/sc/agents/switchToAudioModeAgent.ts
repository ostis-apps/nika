import { client } from '@api/sc';
import { ScAddr, ScConstruction, ScLinkContent, ScTemplate, ScType, ScLinkContentType } from 'ts-sc-client';
import { makeAgent } from '@api/sc/agents/makeAgent';
const actionInitiated = 'action_initiated';
const actionSwitchToAudioMode= 'action_switch_to_audio_mode';
const actionText2Speech='action_text_to_speech'
const rrel1 = 'rrel_1';
const actionFinished = 'action_finished';
const nrelMode= 'nrel_mode';
const conceptTalkingMode='concept_talking_mode'
const conceptPauseMode='concept_pause_mode'
const messageProcessingProgram='message_processing_program'
const actionStandartMessageReply='action_standard_message_reply'
const rrel2 = 'rrel_2';
const nrelGoTo = 'nrel_goto';
const question = 'question';
const actionClass = 'action';
const actionFinishedSuccesfully = 'action_finished_successfully';
const actionFinishedUnsuccesfully = 'action_finished_unsuccessfully';


const baseKeynodes = [
    { id: actionClass, type: ScType.NodeConstClass },
    { id: actionInitiated, type: ScType.NodeConstClass },
    { id: rrel1, type: ScType.NodeConstRole },
    { id: actionFinished, type: ScType.NodeConstClass },
    { id: actionSwitchToAudioMode, type: ScType.NodeConstClass },
    { id: nrelMode, type: ScType.NodeConstNoRole},
    { id: actionSwitchToAudioMode, type: ScType.NodeConstClass },
    { id: conceptTalkingMode, type: ScType.NodeConstClass },
    { id: conceptPauseMode, type: ScType.NodeConstClass },
    { id: messageProcessingProgram, type: ScType.NodeConstStruct},
    { id: actionStandartMessageReply, type: ScType.NodeConstClass},
    { id: rrel2, type: ScType.NodeConstRole},
    { id: nrelGoTo, type: ScType.NodeConstNoRole},
    { id: question, type: ScType.NodeConstClass},
    { id: actionText2Speech, type: ScType.NodeConstClass},
    { id: actionFinishedSuccesfully, type: ScType.NodeConstClass},
    { id: actionFinishedUnsuccesfully, type: ScType.NodeConstClass},

];

const describeAgent = async (mode,
    keynodes: Record<string, ScAddr>,
) => {
    const actionNodeAlias = '_action_node';
    const template = new ScTemplate();
    template.triple(keynodes[actionClass], ScType.EdgeAccessVarPosPerm, [ScType.NodeVar, actionNodeAlias]);
    template.triple(keynodes[actionSwitchToAudioMode], ScType.EdgeAccessVarPosPerm, actionNodeAlias);
    if (mode=="talking")  
        template.tripleWithRelation(
            actionNodeAlias, 
            ScType.EdgeDCommonVar, 
            keynodes[conceptTalkingMode],
            ScType.EdgeAccessVarPosPerm, 
            keynodes[nrelMode]);
    else 
        template.tripleWithRelation(
            actionNodeAlias, 
            ScType.EdgeDCommonVar, 
            keynodes[conceptPauseMode],
            ScType.EdgeAccessVarPosPerm, 
            keynodes[nrelMode]);

    const generationResult = await client.templateGenerate(template, {});
    if (generationResult && generationResult.size > 0) {
        let actionNode = generationResult.get(actionNodeAlias);
        return actionNode;
    }
    return null;
};

export const callSwitchToAudioModeAgent = async (mode) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const actionNode = await describeAgent(mode,keynodes);
    if (actionNode == null) return;
    const construction = new ScConstruction();
    construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes[actionInitiated], actionNode);
    client.createElements(construction);
};

export const check = async(action: ScAddr,keynodes: Record<string, ScAddr>) => {
    const checkTemplate = new ScTemplate();
    checkTemplate.triple(
        keynodes[actionSwitchToAudioMode],
        ScType.EdgeAccessVarPosPerm,
        action,
    );
    const checkResult = await client.templateSearch(checkTemplate);
    if (checkResult.length==0)
        {
            console.log("No switch mode action");
            return false;

        }
    console.log("Switch mode action");
    return true;
}
export const check_mode = async(action:ScAddr,keynodes: Record<string, ScAddr>)=>{
    const checkTemplate = new ScTemplate();
    checkTemplate.tripleWithRelation(
        action,
        ScType.EdgeDCommonVar, 
        keynodes[conceptTalkingMode],
        ScType.EdgeAccessVarPosPerm, 
        keynodes[nrelMode]
    );
    const checkResult = await client.templateSearch(checkTemplate);
    if (checkResult.length==0)
    {
        const secondCheckTemplate = new ScTemplate();
        secondCheckTemplate.tripleWithRelation(
        action,
        ScType.EdgeDCommonVar, 
        keynodes[conceptPauseMode],
        ScType.EdgeAccessVarPosPerm, 
        keynodes[nrelMode]
        );
        const secondCheckResult = await client.templateSearch(secondCheckTemplate);
        if (secondCheckResult.length==0)
            {
                return ""

            }
        console.log("pause");
        return "pause"
    }
    console.log("talking");
    return "talking"

}

export const audio_mode_on = async(keynodes: Record<string, ScAddr>)=>{
    const template = new ScTemplate();
    const _action_standard_message_reply_var="_action_standard_message_reply_var"
    const _decomposition_node="_decomposition_node"
    const _answer_message="_answer_message"
    const _action_text_to_speech_var="_action_text_to_speech_var"


    template.triple(
        keynodes[messageProcessingProgram],
        ScType.EdgeAccessVarPosPerm, 
        [ScType.NodeTuple,_decomposition_node],
    );
    template.triple(
        _decomposition_node,
        ScType.EdgeAccess, 
        [ScType.Node,_action_standard_message_reply_var],
    );
    template.triple(
        keynodes[actionStandartMessageReply],
        ScType.EdgeAccess, 
        _action_standard_message_reply_var,
    )
    template.tripleWithRelation(
        _action_standard_message_reply_var,
        ScType.EdgeAccess, 
        [ScType.Node,_answer_message],
        ScType.EdgeAccess,
        keynodes[rrel2]
    )
    const templateResult1 = await client.templateSearch(template);
    template.tripleWithRelation(
        _action_standard_message_reply_var,
        ScType.EdgeDCommon, 
        [ScType.Node,_action_text_to_speech_var],
        ScType.EdgeAccess,
        keynodes[nrelGoTo]
    )
    const templateResult2 = await client.templateSearch(template);
    if (templateResult1.length!=0)
        {
            if (templateResult2.length!=0)
                {
                console.log("Already on");
                return false
                }
            const action_standard_message_reply_var = templateResult1[0].get(_action_standard_message_reply_var);
            const decomposition_node = templateResult1[0].get(_decomposition_node);
            const answer_message = templateResult1[0].get(_answer_message);
            if (await update_message_processing_programm(action_standard_message_reply_var,decomposition_node,answer_message,keynodes)) return true
            return false

        }
        return false
    
    }

export const update_message_processing_programm= async(action_example,node_var_tuple,answer_message,keynodes: Record<string, ScAddr>)=>{
    const _action_tts_var="_action_tts_var"
    const _edge_decomposition_of_action_membership="_edge_decomposition_of_action_membership"
    const _edge_goto_realtion="_edge_goto_realtion"
    const _edge_nrel_goto_membership="_edge_nrel_goto_membership"
    const _edge_action_tts_membership="_edge_action_tts_membership"
    const _edge_action_membership="_edge_action_membership"
    const _edge_argument_relation="_edge_argument_relation"
    const _edge_rrel1_membership="_edge_rrel1_membership"
    const updatedConstruction = new ScConstruction(); 
    updatedConstruction.createNode(ScType.NodeVar,_action_tts_var); //1
    updatedConstruction.createEdge(ScType.EdgeDCommonVar,action_example,_action_tts_var,_edge_goto_realtion);                                                     
    updatedConstruction.createEdge(ScType.EdgeAccessVarPosPerm,keynodes[nrelGoTo],_edge_goto_realtion,_edge_nrel_goto_membership);                                                     
    updatedConstruction.createEdge(ScType.EdgeAccessVarPosPerm,node_var_tuple,_action_tts_var,_edge_decomposition_of_action_membership);    //2                                                
    updatedConstruction.createEdge(ScType.EdgeAccessVarPosPerm,keynodes[actionText2Speech],_action_tts_var,_edge_action_tts_membership);    //3                                                
    updatedConstruction.createEdge(ScType.EdgeAccessVarPosPerm,keynodes[actionClass],_action_tts_var,_edge_action_membership);    //4                                            
    updatedConstruction.createEdge(ScType.EdgeAccessVarPosPerm,_action_tts_var,answer_message,_edge_argument_relation);    //5                                           
    updatedConstruction.createEdge(ScType.EdgeAccessVarPosPerm,keynodes[rrel1],_edge_argument_relation,_edge_rrel1_membership);       
    updatedConstruction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[messageProcessingProgram],_action_tts_var);       
    updatedConstruction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[messageProcessingProgram],keynodes[actionText2Speech]);       
    updatedConstruction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[messageProcessingProgram],_edge_goto_realtion);       
    updatedConstruction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[messageProcessingProgram],_edge_decomposition_of_action_membership);       
    updatedConstruction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[messageProcessingProgram],_edge_nrel_goto_membership);       
    updatedConstruction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[messageProcessingProgram],_edge_action_tts_membership);       
    updatedConstruction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[messageProcessingProgram],_edge_action_membership);       
    updatedConstruction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[messageProcessingProgram],_edge_argument_relation);       
    updatedConstruction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[messageProcessingProgram],_edge_rrel1_membership);       
    const res = await client.createElements(updatedConstruction);                                                                 //                                                    множестов
    if (res && res.length > 0) {
        return true
    }
    return false
}

export const audio_mode_off = async(keynodes: Record<string, ScAddr>)=>{
    const template = new ScTemplate();
    const _action_text_to_speech_var="_action_text_to_speech_var"
    const _to_tts="_to_tts"

    template.triple(
        keynodes[messageProcessingProgram],
        ScType.EdgeAccessVarPosPerm,
        [ScType.Node,_action_text_to_speech_var],
    );

    template.triple(
        keynodes[messageProcessingProgram],
        [ScType.EdgeAccessVarPosPerm, _to_tts],
        keynodes[actionText2Speech],
    );

    template.triple(
        keynodes[actionText2Speech],
        ScType.EdgeAccess,
        _action_text_to_speech_var,
    );
    const templateResult = await client.templateSearch(template);
    if (templateResult.length==0)
        {
            console.log("Already off");
            return false

        }
    const action_tts_example = templateResult[0].get(_action_text_to_speech_var);
    const to_tts = templateResult[0].get(_to_tts);
    const res = await client.deleteElements([action_tts_example,to_tts]);
    if (res) {
        return true
    }
    return false
    }

export const switchToAudioModeAgent = async(subscribedAddr: ScAddr, foundEgde: ScAddr, action: ScAddr) => {
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    if(await check(action,keynodes)===false) 
        {
            return;
        }
    const mode=check_mode(action,keynodes);
    if (await mode==="talking") 
        {
         if (await audio_mode_on(keynodes))
         {
            const construction = new ScConstruction();
            construction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[actionFinishedSuccesfully], action);
            client.createElements(construction);
         }
        }  
    if (await mode==="pause") 
        {
            if (await audio_mode_off(keynodes))
            {
                const construction = new ScConstruction();
                construction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[actionFinishedSuccesfully], action);
                client.createElements(construction);
            }
        } 
    const construction = new ScConstruction();
    construction.createEdge(ScType.EdgeAccessConstPosPerm,keynodes[actionFinishedUnsuccesfully], action);
    client.createElements(construction);
    return;
};