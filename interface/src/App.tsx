import { lazy, useEffect, useState } from "react";
import { Route, Redirect } from "react-router-dom";
import { loadingComponent } from '@components/LoadingComponent';
import { routes } from '@constants';
import { client } from "@api";
import { ScEventSubscriptionParams, ScEventType, ScTemplate, ScType } from "ts-sc-client";

import 'antd/dist/antd.css';
import './assets/main.css';

import { Layout } from 'antd';
const { Header, Content, Footer } = Layout;

import { HeaderPanel } from "@components/Header";
import { FooterPanel } from "@components/Footer";

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
                { id: conceptHeader, type: ScType.ConstNodeClass },
                { id: conceptMainPart, type: ScType.ConstNodeClass },
                { id: conceptFooter, type: ScType.ConstNodeClass },
            ];

            const helpKeynodes = [
                { id: componentColor, type: ScType.ConstNodeNonRole },
            ];

            const colorAlias = '_color';
            const componentAlias = '_component'
             
            const keynodes = await client.resolveKeynodes(baseKeynodes);
            const hKeynodes = await client.resolveKeynodes(helpKeynodes);

            for (var i = 0; i < baseKeynodes.length; i++) {
                const template = new ScTemplate();
                template.triple(
                    keynodes[baseKeynodes[i].id],
                    ScType.VarPermPosArc,
                    [ScType.VarNode, componentAlias],
                );
                template.quintuple(
                    componentAlias,
                    ScType.VarCommonArc,
                    [ScType.VarNodeLink, colorAlias],
                    ScType.VarPermPosArc,
                    hKeynodes[componentColor],
                );
                const resultColorLink = await client.searchByTemplate(template);
                
                if (resultColorLink.length) {
                    const colorLink = resultColorLink[0].get(colorAlias);
                    const resultColor = await client.getLinkContents([colorLink]);
                    if (resultColor.length) {
                        let color = resultColor[0].data;
                        funcChange[i](color as any);
                        const eventParams = new ScEventSubscriptionParams(colorLink, ScEventType.BeforeChangeLinkContent, fetchColorValue);
                        await client.createElementaryEventSubscriptions([eventParams]); 
                    }
                }    
            }
        }
    
        useEffect(() => {
            fetchColorValue();
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
