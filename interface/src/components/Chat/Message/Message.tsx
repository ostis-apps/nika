import { PropsWithChildren } from 'react';
import {
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
import styled from 'styled-components';
import { getFontSizeFromSettings } from '@api/sc/checkUser';

interface IProps {
    isLoading?: boolean;
    isLeft?: boolean;
    isError?: boolean;
    time?: string | number;
    attachment?: string;
    onClick?: () => void;
    accentColor: string;
    theme: string;
    fontSize: string;
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
    theme,
    accentColor,
    fontSize,
    onClick,
}: PropsWithChildren<IProps>) => {
    console.log(accentColor);
    const WrapperMessage = styled.div<{ isLeft: boolean }>`
        position: relative;
        width: fit-content;
        max-width: 70%;
        margin: 0px 18px 8px 16px;
        padding: 12px 12px 12px 12px;
        align-self: ${(props) => (props.isLeft ? 'start' : 'end')};
        background: ${(props) =>
            props.isLeft ? (theme == 'dark' ? '#202221' : '#ffffff30') : theme == 'dark' ? accentColor : '#00000060'};
        border-radius: 20px;
        word-break: break-word;

        &:first-child {
            margin-top: auto;
        }

        a {
            color: ${accentColor} !important;
        }
    `;
    const hookLanguage = useLanguage();

    return (
        <>
            <>
                <WrapperMessage isLeft={isLeft}>
                    <Text style={{ fontSize: getFontSizeFromSettings(fontSize) }}>
                        <TextWrapper> {children}</TextWrapper>
                    </Text>
                    <Info>
                        <Time style={{ fontWeight: 700 }}>{time}</Time>
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
