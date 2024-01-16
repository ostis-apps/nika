import React, { useEffect, useState, useReducer } from 'react';
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
import { client } from "@api";
import { ScAddr } from 'ts-sc-client';
import { ScTemplate, ScType } from 'ts-sc-client';

type DesireDesc = {
    title: string;
    img: string;
    desireAddr: ScAddr;
    isSelected: boolean;
};

export const Intro = () => {
    const [desires, setDesires] = useState<DesireDesc[]>([]);
    const [, forceUpdate] = useReducer(x => x+1, 0);

    const findKb = async () => {
        const conceptInfrastructure = 'concept_infrastructure';
            const langRu = 'lang_ru';
            const componentInclusion = 'nrel_inclusion'
            const componentNrelMainIdtf = 'nrel_main_idtf'
        
            const baseKeynodes = [
                { id: conceptInfrastructure, type: ScType.NodeConstClass },
                { id: langRu, type: ScType.NodeConstClass },
            ];
        
            const helpKeynodes = [
                { id: componentInclusion, type: ScType.NodeConstNoRole },
                { id: componentNrelMainIdtf, type: ScType.NodeConstNoRole },
            ];
            const keynodes = await client.resolveKeynodes(baseKeynodes);
            const hKeynodes = await client.resolveKeynodes(helpKeynodes);
        
            const typeDesiresClass = '_type_desires'
            const desire = '_desire'
            const desireText = '_desire_text'

            const template = new ScTemplate();
            template.tripleWithRelation(
                keynodes[conceptInfrastructure],
                ScType.EdgeDCommonVar,
                [ScType.NodeVarClass, typeDesiresClass],
                ScType.EdgeAccessVarPosPerm,
                hKeynodes[componentInclusion],
            );
            template.triple(
                typeDesiresClass,
                ScType.EdgeAccessVarPosPerm,
                [ScType.NodeVarClass, desire]
            );
            template.tripleWithRelation(
                desire,
                ScType.EdgeDCommonVar,
                [ScType.LinkVar, desireText],
                ScType.EdgeAccessVarPosPerm,
                hKeynodes[componentNrelMainIdtf],
            );
            template.triple(
                keynodes[langRu],
                ScType.EdgeAccessVarPosPerm,
                desireText,
            );
            const result = await client.templateSearch(template);
            
            let a: DesireDesc[] = []
            for (let i = 0; i < result.length; i++) {
                const linkTitle = result[i].get(desireText);
                const resTitle = await client.getLinkContents([linkTitle]);
                const title = resTitle[0].data.toString();

                const desireAddr = result[i].get(desire);
                const desireObj: DesireDesc = {title:title, img:'', desireAddr:desireAddr, isSelected:false};
                a.push(desireObj);
            }
            setDesires( a )
    }

    useEffect(() => {
        (async () => {
            findKb()   
        })();
    }, [desires]);

    const select = (item, index) => {
        desires[index].isSelected = true;
        forceUpdate();
    }

    const opacity = {
        "opacity": "1",
    }
   
    return (<div>
    <BackgroundCircle/>
        <IntroWrapper>
            <WrapperContentIntro>
                <div>
                    <HelloTextIntro>Здравствуйте</HelloTextIntro>
                    <NameInput type="text" placeholder="Имя"></NameInput>
                </div>
                <MainBtnsIntro>
                    { desires.map((item, index) => { return (
                        <DesireButton key={ index } onClick={() => select(item, index)}>
                            <TextButton>{ item.title }</TextButton>
                            <LinerBtns></LinerBtns>
                            <SelectMask style={ item.isSelected ? opacity:undefined}>✓</SelectMask>
                        </DesireButton>  )
                    }) }
                </MainBtnsIntro>
                <SaveButton>Сохранить</SaveButton>
            </WrapperContentIntro>
        </IntroWrapper>
    </div>);
};