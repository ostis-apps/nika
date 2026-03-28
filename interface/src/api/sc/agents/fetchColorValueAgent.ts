import { 
    ScEventSubscriptionParams, 
    ScEventType, 
    ScTemplate, 
    ScType 
} from "ts-sc-client";
import { client } from "@api";


export async function fetchColorValue(funcChange: ((color: string) => void)[]) {
    
    const conceptHeader = 'concept_header';
    const conceptFooter = 'concept_footer';
    const conceptMainPart = 'concept_main_part';
    const componentColor = 'nrel_component_color';

    const baseKeynodes = [
        { id: conceptHeader, type: ScType.ConstNodeClass },
        { id: conceptMainPart, type: ScType.ConstNodeClass },
        { id: conceptFooter, type: ScType.ConstNodeClass },
    ];

    const helpKeynodes = [
        { id: componentColor, type: ScType.ConstNodeNonRole },
    ];

    const colorAlias = '_color';
    const componentAlias = '_component'
        
    const keynodes = await client.resolveKeynodes(baseKeynodes);
    const hKeynodes = await client.resolveKeynodes(helpKeynodes);

    for (var i = 0; i < baseKeynodes.length; i++) {
        const template = new ScTemplate();
        template.triple(
            keynodes[baseKeynodes[i].id],
            ScType.VarPermPosArc,
            [ScType.VarNode, componentAlias],
        );
        template.quintuple(
            componentAlias,
            ScType.VarCommonArc,
            [ScType.VarNodeLink, colorAlias],
            ScType.VarPermPosArc,
            hKeynodes[componentColor],
        );
        const resultColorLink = await client.searchByTemplate(template);
        
        if (resultColorLink.length) {
            const colorLink = resultColorLink[0].get('_color');
            const resultColor = await client.getLinkContents([colorLink]);
            if (resultColor.length) {
                let color = resultColor[0].data;
                
                if (funcChange[i] && typeof funcChange[i] === 'function') {
                    funcChange[i](color as string);
                } else {
                    console.error(`funcChange[${i}] is not a function:`, funcChange[i]);
                }
                
                const eventParams = new ScEventSubscriptionParams(
                    colorLink, 
                    ScEventType.BeforeChangeLinkContent, 
                    () => fetchColorValue(funcChange)
                );
                await client.createElementaryEventSubscriptions([eventParams]); 
            }
        }    
    }
}