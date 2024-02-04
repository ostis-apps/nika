import { PropsWithChildren, useEffect, useState } from 'react';
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
import { ScAddr } from 'ts-sc-client';
import Cookie from 'universal-cookie';
import { getUserSettings } from '@api/sc/checkUser';
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
    const [userTheme, setUserTheme] = useState<String>('dark');
    const [accentColor, setAccentColor] = useState<String>('light');

    useEffect(() => {
        (async () => {
            const cookie = new Cookie();
            const userAddr = cookie.get('userAddr')
                ? new ScAddr(parseInt(String(cookie.get('userAddr'))))
                : new ScAddr(0);

            const userSettings = await getUserSettings(userAddr);
            setUserTheme(userSettings['nrel_theme']);
            setAccentColor(userSettings['nrel_accent_color']);
        })();
    });

    const WrapperMessage = styled.div<{ isLeft: boolean }>`
        position: relative;
        width: fit-content;
        max-width: 70%;
        margin: 0px 18px 8px 16px;
        padding: 12px 12px 12px 12px;
        align-self: ${(props) => (props.isLeft ? 'start' : 'end')};
        background: ${(props) =>
            props.isLeft
                ? userTheme == 'dark'
                    ? '#333436'
                    : '#33343690'
                : userTheme == 'dark'
                ? (accentColor as string)
                : 'rgba(256, 256, 256, 0.3)'};

        border-radius: 20px;
        word-break: break-word;

        &:first-child {
            margin-top: auto;
        }

        a {
            color: ${accentColor as string} !important;
        }
    `;

    return (
        <>
            <>
                <WrapperMessage isLeft={isLeft}>
                    <Text>
                        <TextWrapper> {children}</TextWrapper>
                    </Text>
                    <Info>
                        <Time>{time}</Time>
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
