import { useEffect, useState } from "react";
import { client } from "@api";
import { ScEventParams, ScEventType, ScTemplate, ScType } from "ts-sc-client";


export const FooterPanel = () => {

    const [footerText, setFooterText] = useState<string>('Авторское право © Intelligent Semantic Systems LLC, Все права защищены');
    const [systemNameSize, setSystemNameSize] = useState<number>(22);

    async function fetchTextValue() {
        const conceptFooter = 'concept_footer';
        const copyrightText = 'nrel_copyright_text';
        const fontSizeRel = 'nrel_font_size';

        const baseKeynodes = [
            { id: conceptFooter, type: ScType.NodeConstClass },
        ];

        const helpKeynodes = [
            { id: copyrightText, type: ScType.NodeConstNoRole },
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
                hKeynodes[copyrightText],
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
                    setFooterText(text as any);
                    setSystemNameSize(size as number);
                    const eventParams = new ScEventParams(textLink, ScEventType.ChangeContent, fetchTextValue);
                    await client.eventsCreate([eventParams]); 
                }
            }    
        }
    }

    const copyrightStyles = {
        'font-size': systemNameSize,
    };

    useEffect(() => {
        fetchTextValue();
    }, []);

    return (
        <span className="copyright" /*style={copyrightStyles}*/>
            {footerText}
        </span>
    );
}
