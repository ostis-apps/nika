import { PropsWithChildren } from 'react';
import {
    WrapperMessage,
    Time,
    Text,
    WrapperRebootError,
    TextReboot,
    Img,
    ImgWrapper,
    Info,
    TextWrapper,
    WrapperLoadingIcon,
} from './styled';
import { ReactComponent as LoadingIcon } from '@assets/icon/messageLoading.svg';
import { ReactComponent as ErrorIcon } from '@assets/icon/errorMessage-icon.svg';
import { ReactComponent as RebootIcon } from '@assets/icon/rebootErrorMessage-icon.svg';
import { useLanguage } from '@hooks/useLanguage';
import { ScEventParams, ScEventType, ScTemplate, ScType } from "ts-sc-client";
import { client } from "@api";
import { useEffect, useState } from "react";

interface IProps {
    isLoading?: boolean;
    isLeft?: boolean;
    isError?: boolean;
    time?: string | number;
    attachment?: string;
    onClick?: () => void;
}

const textSend = {
    en: 'Send again',
    ru: 'Повторить отправку',
};

export const Message = ({
    isLeft = false,
    isError = false,
    isLoading = false,
    time = '',
    children,
    onClick,
}: PropsWithChildren<IProps>) => {
    const hookLanguage = useLanguage();

    const [userBgColor, setUserBgColor] = useState<string>('#E2E9E7');
    const [systemBgColor, setSystemBgColor] = useState<string>('#DFDBD0');
    const [messageTextColor, setMessageTextColor] = useState<string>('#000000');
    const [timeColor, settimeColor] = useState<string>('#696969');
    const funcChange = [setUserBgColor, setSystemBgColor, setMessageTextColor, settimeColor]

    async function fetchColorValue() {
        const conceptSentMessage = 'concept_sent_message';
        const conceptReceivedMessage = 'concept_received_message';
        const componentColor = 'nrel_component_color';
        const conceptMessageText = 'concept_text_of_message';
        const conceptTime = 'concept_time';
    
        const baseKeynodes = [
            { id: conceptSentMessage, type: ScType.NodeConstClass },
            { id: conceptReceivedMessage, type: ScType.NodeConstClass },
            { id: conceptMessageText, type: ScType.NodeConstClass },
            { id: conceptTime, type: ScType.NodeConstClass }
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

    const messageTextStyles = {
        color: messageTextColor,
    }; 

    const timeStyles = {
        color: timeColor,
    };

    return (
        <>
            <>
                <WrapperMessage isLeft={isLeft} color1={systemBgColor} color2={userBgColor}>
                    <Text style={ messageTextStyles }>
                        <TextWrapper> {children}</TextWrapper>
                    </Text>
                    <Info>
                        <Time style={ timeStyles }>{time}</Time>
                    </Info>
                    <WrapperLoadingIcon>
                        {isLoading && !isLeft && <LoadingIcon />}
                        {isError && !isLeft && <ErrorIcon />}
                    </WrapperLoadingIcon>
                </WrapperMessage>
            </>

            {isError && !isLeft && (
                <WrapperRebootError onClick={onClick}>
                    <RebootIcon />
                    <TextReboot>{textSend[hookLanguage]}</TextReboot>
                </WrapperRebootError>
            )}
        </>
    );
};
