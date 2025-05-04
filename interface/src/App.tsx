import { lazy, useEffect, useState } from "react";
import { Route, Redirect } from "react-router-dom";
import { loadingComponent } from '@components/LoadingComponent';
import { routes } from '@constants';
import { client } from "@api";
import { ScAddr, ScEventParams, ScEventType, ScTemplate, ScType } from "ts-sc-client";

import 'antd/dist/antd.css';
import './assets/main.css';

import { Layout } from 'antd';
const { Header, Content, Footer } = Layout;

import { HeaderPanel } from "@components/Header";
import { FooterPanel } from "@components/Footer";
import { audioAgent } from "@agents/audioAgent";
import { switchToAudioModeAgent } from "@agents/switchToAudioModeAgent"

const Demo = loadingComponent(lazy(() => import('@pages/Demo')));
const About = loadingComponent(lazy(() => import('@pages/About')));

const DemoRoutes = () => (
    <>
        <Route exact path={routes.MAIN} component={Demo} />
    </>
);

const AboutRoutes = () => (
    <>
        <Route path={routes.ABOUT}>
            <About />
        </Route>
    </>
);

export const App = () => {
        const [headerBgColor, setHeaderBgColor] = useState<string>('#39494C');
        const [mainBgColor, setMainBgColor] = useState<string>('#fcfafa');
        const [footerBgColor, setFooterBgColor] = useState<string>('#39494C');
        
        const funcChange = [setHeaderBgColor, setMainBgColor, setFooterBgColor]

        async function fetchColorValue() {
            const conceptHeader = 'concept_header';
            const conceptFooter = 'concept_footer';
            const conceptMainPart = 'concept_main_part';
            const componentColor = 'nrel_component_color';

            const baseKeynodes = [
                { id: conceptHeader, type: ScType.NodeConstClass },
                { id: conceptMainPart, type: ScType.NodeConstClass },
                { id: conceptFooter, type: ScType.NodeConstClass },
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
        async function registerAudioAgent() {
            const actionInitiated = "action_initiated";
            const keynodes = [
                { id: actionInitiated, type: ScType.NodeConstNoRole },
            ];

            const keynodesAddrs = await client.resolveKeynodes(keynodes);

            const eventParams = new ScEventParams(keynodesAddrs[actionInitiated], ScEventType.AddOutgoingEdge, audioAgent);
            await client.eventsCreate([eventParams]); 
        }

        async function registerSwitchToAudioModeAgent() {
            const actionInitiated = "action_initiated";
            const keynodes = [
                { id: actionInitiated, type: ScType.NodeConstNoRole },
            ];

            const keynodesAddrs = await client.resolveKeynodes(keynodes);

            const eventParams = new ScEventParams(
                keynodesAddrs[actionInitiated], 
                ScEventType.AddOutgoingEdge, 
                switchToAudioModeAgent);
            await client.eventsCreate([eventParams]); 
        }

        useEffect(() => {
            fetchColorValue();
            registerAudioAgent();
            registerSwitchToAudioModeAgent();
        }, []);
    
        const headerStyles = {
            background: headerBgColor,
        };
        
        const mainStyles = {
            background: mainBgColor,
        };

        const footerStyles = {
            background: footerBgColor,
        };

    return (
        <Layout>
            <Header style={ headerStyles }>
                <HeaderPanel />
            </Header>
            <Content style={ mainStyles }>
                <DemoRoutes />
                <AboutRoutes />
            </Content>
            <Footer style={ footerStyles }>
                <FooterPanel />
            </Footer>
        </Layout>
    );
};
