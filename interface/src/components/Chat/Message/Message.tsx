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
