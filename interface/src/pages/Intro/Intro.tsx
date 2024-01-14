import React, { useEffect } from 'react';
import {
    BackgroundCircle,
    IntroWrapper,
    WrapperContentIntro,
    HelloTextIntro,
    NameInput,
    MainBtnsIntro,
    DesireButton,
    LinerBtns,
    TextButton,
    SelectMask,
    SaveButton,    
} from './styled'

export const Intro = () => {
    return (<div>
    <BackgroundCircle/>
        <IntroWrapper>
            <WrapperContentIntro>
                <div>
                    <HelloTextIntro>Здравствуйте</HelloTextIntro>
                    <NameInput type="text" placeholder="Имя"></NameInput>
                </div>
                <MainBtnsIntro>
                    <DesireButton>
                        <TextButton>театр</TextButton>
                        <LinerBtns></LinerBtns>
                        <SelectMask>✓</SelectMask>
                    </DesireButton>  
                </MainBtnsIntro>
                <SaveButton>Сохранить</SaveButton>
            </WrapperContentIntro>
        </IntroWrapper>
    </div>);
};