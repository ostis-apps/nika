import { useLanguage } from '@hooks/useLanguage';
import styled from 'styled-components';
import { useEffect, useState } from "react";
import { ScEventParams, ScEventType, ScTemplate, ScType } from "ts-sc-client";
import { client } from "@api";

interface IProps {
    date: string;
}
const Wrapper = styled.div`
    width: 140px;
    margin: auto;
    margin-top: 8px;
    margin-bottom: 16px;
    padding: 6px 10px;
    background: #8fc1cf;
    border-radius: 10px;
    color: black;
    font-weight: 400;
    font-size: 14px;
    line-height: 16px;
`;
const DateInfo = styled.div`
    align-items: center;
    text-align: center;
`;

const months = {
    en: {
        1: 'january',
        2: 'february',
        3: 'march',
        4: 'april',
        5: 'mai',
        6: 'june',
        7: 'july',
        8: 'august',
        9: 'september',
        10: 'october',
        11: 'november',
        12: 'december',
    },
    ru: {
        1: 'января',
        2: 'февраля',
        3: 'марта',
        4: 'апреля',
        5: 'мая',
        6: 'июня',
        7: 'июля',
        8: 'августа',
        9: 'сентября',
        10: 'октября',
        11: 'ноября',
        12: 'декабря',
    },
};

export const Date = ({ date }: IProps) => {
    const hookLanguage = useLanguage();
    const [year, month, day] = date.toString().split('.');
    const currentMonth = months[hookLanguage];

    const [dateBgColor, setDateBgColor] = useState<string>('#8fc1cf');
    const [dateTextColor, setDateTextColor] = useState<string>('#000000');
    const funcChange = [setDateBgColor, setDateTextColor]

    async function fetchColorValue() {
        const conceptDate = 'concept_date';
        const conceptDateText = 'concept_date_text';
        const componentColor = 'nrel_component_color';
        const baseKeynodes = [
            { id: conceptDate, type: ScType.NodeConstClass },
            { id: conceptDateText, type: ScType.NodeConstClass },
        ];
        const helpKeynodes = [
            { id: componentColor, type: ScType.NodeConstNoRole },
        ];
        const colorAlias = '_color';
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
                [ScType.LinkVar, colorAlias],
                ScType.EdgeAccessVarPosPerm,
                hKeynodes[componentColor],
            );
            const resultColorLink = await client.templateSearch(template);
            
            if (resultColorLink.length) {
                const colorLink = resultColorLink[0].get(colorAlias);
                const resultColor = await client.getLinkContents([colorLink]);
                if (resultColor.length) {
                    let color = resultColor[0].data;
                    funcChange[i](color as any);
                    const eventParams = new ScEventParams(colorLink, ScEventType.ChangeContent, fetchColorValue);
                    await client.eventsCreate([eventParams]); 
                }
            }    
        }
    }
    useEffect(() => {
        fetchColorValue();
    }, []);

    
    const dateStyles = {
        background: dateBgColor,
        color: dateTextColor
    };

    return (
        <>
            <Wrapper style={ dateStyles }>
                <DateInfo>{`${day} ${currentMonth[Number(month)]} ${year}`}</DateInfo>
            </Wrapper>
        </>
    );
};
