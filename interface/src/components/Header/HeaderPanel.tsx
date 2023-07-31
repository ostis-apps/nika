import { routes } from '@constants';
import { NavLink } from 'react-router-dom';
import { useEffect, useState } from "react";
import { client } from "@api";
import { ScEventParams, ScEventType, ScTemplate, ScType } from "ts-sc-client";

export const HeaderPanel = () => {
    const [systemName, setSystemName] = useState<string>('NIKA');
    const [systemNameSize, setSystemNameSize] = useState<number>(22);

    const [menuFirst, setMenuFirst] = useState<string>('Главное');
    const [menuSecond, setMenuSecond] = useState<string>('О нас');

    async function fetchSystemNameValue() {
        const conceptSysName = 'concept_decorative_user_interface_component';
        const systemNameRel = 'nrel_system_name';
        const fontSizeRel = 'nrel_font_size';
        

        const baseKeynodes = [
            { id: conceptSysName, type: ScType.NodeConstClass },
        ];

        const helpKeynodes = [
            { id: systemNameRel, type: ScType.NodeConstNoRole },
            { id: fontSizeRel, type: ScType.NodeConstNoRole },
        ];

        const textAlias = '_text';
        const fontSize = '_size';
        const componentAlias = '_component'
         
        const keynodes = await client.resolveKeynodes(baseKeynodes);
        const hKeynodes = await client.resolveKeynodes(helpKeynodes);

        for (var i = 0; i < baseKeynodes.length; i++) {
            const template = new ScTemplate();
            template.triple(
                keynodes[baseKeynodes[i].id],
                ScType.EdgeAccessVarPosPerm,
                [ScType.NodeVar, componentAlias],
            );
            template.tripleWithRelation(
                componentAlias,
                ScType.EdgeDCommonVar,
                [ScType.LinkVar, textAlias],
                ScType.EdgeAccessVarPosPerm,
                hKeynodes[systemNameRel],
            );
            template.tripleWithRelation(
                componentAlias,
                ScType.EdgeDCommonVar,
                [ScType.LinkVar, fontSize],
                ScType.EdgeAccessVarPosPerm,
                hKeynodes[fontSizeRel],
            );
            const resultTextLink = await client.templateSearch(template);
            
            if (resultTextLink.length) {
                const textLink = resultTextLink[0].get(textAlias);
                const resultText = await client.getLinkContents([textLink]);
                const sizeLink = resultTextLink[0].get(fontSize);
                const resultSize = await client.getLinkContents([sizeLink]);
                if (resultText.length && resultSize.length) {
                    let text = resultText[0].data;
                    let size = resultText[0].data;
                    setSystemName(text as any);
                    setSystemNameSize(size as number);
                    const eventParams = new ScEventParams(textLink, ScEventType.ChangeContent, fetchSystemNameValue);
                    await client.eventsCreate([eventParams]); 
                }
            }    
        }
    }

    async function fetchMenuValues() {
        const conceptMenu = 'concept_menu';
        const menuButtonTextRel = 'nrel_menu_button_text';
        const decompositionRel = 'nrel_decomposition';
        const rrel1 = 'rrel_1';
        const rrel2 = 'rrel_2';

        const baseKeynodes = [
            { id: conceptMenu, type: ScType.NodeConstClass },
        ];

        const helpKeynodes = [
            { id: menuButtonTextRel, type: ScType.NodeConstNoRole },
            { id: decompositionRel, type: ScType.NodeConstNoRole },
            { id: rrel1, type: ScType.NodeConstRole },
            { id: rrel2, type: ScType.NodeConstRole },
        ];


        const firstNode = '_first_node';
        const secondNode = '_second_node';
        const textFirstAlias = '_text_first';
        const textSecondAlias = '_text_second';
        const decompTuple = '_tuple';
        const componentAlias = '_component'
         
        const keynodes = await client.resolveKeynodes(baseKeynodes);
        const hKeynodes = await client.resolveKeynodes(helpKeynodes);

        for (var i = 0; i < baseKeynodes.length; i++) {
            const template = new ScTemplate();
            template.triple(
                keynodes[baseKeynodes[i].id],
                ScType.EdgeAccessVarPosPerm,
                [ScType.NodeVar, componentAlias],
            );
            template.tripleWithRelation(
                [ScType.NodeVarTuple, decompTuple],
                ScType.EdgeDCommonVar,
                componentAlias,
                ScType.EdgeAccessVarPosPerm,
                hKeynodes[decompositionRel],
            );
            template.tripleWithRelation(
                decompTuple,
                ScType.EdgeAccessVarPosPerm,
                [ScType.NodeVar, firstNode],
                ScType.EdgeAccessVarPosPerm,
                hKeynodes[rrel1],
            );
            template.tripleWithRelation(
                firstNode,
                ScType.EdgeDCommonVar,
                [ScType.LinkVar, textFirstAlias],
                ScType.EdgeAccessVarPosPerm,
                hKeynodes[menuButtonTextRel],
            );
            template.tripleWithRelation(
                decompTuple,
                ScType.EdgeAccessVarPosPerm,
                [ScType.NodeVar, secondNode],
                ScType.EdgeAccessVarPosPerm,
                hKeynodes[rrel2],
            );
            template.tripleWithRelation(
                secondNode,
                ScType.EdgeDCommonVar,
                [ScType.LinkVar, textSecondAlias],
                ScType.EdgeAccessVarPosPerm,
                hKeynodes[menuButtonTextRel],
            );
            const resultTextLink = await client.templateSearch(template);
            
            if (resultTextLink.length) {
                const textFirstLink = resultTextLink[0].get(textFirstAlias);
                const resultFirstText = await client.getLinkContents([textFirstLink]);
                const textSecondLink = resultTextLink[0].get(textSecondAlias);
                const resultSecondText = await client.getLinkContents([textSecondLink]);

                if (resultFirstText.length && resultSecondText.length) {
                    let firstText = resultFirstText[0].data;
                    let secondText = resultSecondText[0].data;
                    setMenuFirst(firstText as any);
                    setMenuSecond(secondText as any);
                    const eventParams = new ScEventParams(textFirstLink, ScEventType.ChangeContent, fetchMenuValues) || new ScEventParams(textSecondLink, ScEventType.ChangeContent, fetchMenuValues);
                    await client.eventsCreate([eventParams]); 
                }
            }    
        }
    }


    const headerLogoStyles = {
        'font-size': systemNameSize,
    };

    useEffect(() => {
        fetchSystemNameValue();
        fetchMenuValues();
    }, []);


    return (
        <div className="header">
            <h1 className="header-logo-text" /*style={headerLogoStyles}*/>{systemName}</h1>
            <div className="nav-container">
                <ul className="nav">
                    <li>
                        <NavLink to={routes.MAIN}>{menuFirst}</NavLink>
                    </li>
                    <li>
                        <NavLink to={routes.ABOUT}>{menuSecond}</NavLink>
                    </li>
                </ul>
            </div>
        </div>
    );
}
