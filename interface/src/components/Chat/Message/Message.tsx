import { PropsWithChildren, useState  } from 'react';
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
    SpeakButton,
} from './styled';
import { ReactComponent as LoadingIcon } from '@assets/icon/messageLoading.svg';
import { ReactComponent as ErrorIcon } from '@assets/icon/errorMessage-icon.svg';
import { ReactComponent as RebootIcon } from '@assets/icon/rebootErrorMessage-icon.svg';
import { ReactComponent as TextToSpeechIcon } from '@assets/icon/TextToSpeech-icon.svg';
import { ReactComponent as PauseIcon } from '@assets/icon/pause-icon.svg';
import { useLanguage } from '@hooks/useLanguage';
import { ScAddr, ScEventParams, ScEventType, ScType ,ScConstruction} from 'ts-sc-client';
import { callText2SpeechAgent } from '@api/sc/agents/audioAgent';
declare var responsiveVoice: any;
interface IProps {
    addr:ScAddr;
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
    addr,
    isLeft = false,
    isError = false,
    isLoading = false,
    time = '',
    children,
    onClick,
}: PropsWithChildren<IProps>) => {
  const hookLanguage = useLanguage();
  const [isTalking, setIsTalking] = useState(false);
    const talking = async () => {
        if (isTalking) {
            setIsTalking(false);
            responsiveVoice.cancel();
        }else{
            setIsTalking(true);
            await callText2SpeechAgent(addr);
            const checkIfSpeaking = setInterval(() => {
                if(!responsiveVoice.isPlaying())
                {
                    setIsTalking(false);
                    clearInterval(checkIfSpeaking);
                }
            },2200);
        }
        
    };   
     
    
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
                    {isLeft && (
                    <SpeakButton onClick={talking}>
                    {isTalking ? (
                        <PauseIcon width="20px" height="20px" />
                    ) : (
                        <TextToSpeechIcon width="20px" height="20px" />
                    )}
                </SpeakButton>  )}
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
