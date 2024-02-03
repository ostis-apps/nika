import React, { lazy, useEffect, useState } from 'react';
import { Route } from 'react-router-dom';
import { loadingComponent } from '@components/LoadingComponent';
import { routes } from '@constants';
import { client } from '@api';
import { ScAddr, ScEventParams, ScEventType, ScTemplate, ScType } from 'ts-sc-client';

import 'antd/dist/antd.css';
import './assets/main.css';

import { Layout } from 'antd';
const { Header, Content, Footer } = Layout;

import { HeaderPanel } from '@components/Header';
import { FooterPanel } from '@components/Footer';

const Demo = loadingComponent(lazy(() => import('@pages/Demo')));
const MapPage = loadingComponent(lazy(() => import('@pages/MapPage')));
const Intro = loadingComponent(lazy(() => import('@pages/Intro')));
const Reg = loadingComponent(lazy(() => import('@pages/Registration')));
const Login = loadingComponent(lazy(() => import('@pages/Login')));
const Home = loadingComponent(lazy(() => import('@pages/Home')));
const Profile = loadingComponent(lazy(() => import('@pages/Profile')));
const Settings = loadingComponent(lazy(() => import('@pages/Settings')));
const Saved = loadingComponent(lazy(() => import('@pages/Saved')));
const OwnMap = loadingComponent(lazy(() => import('@pages/OwnMap')));

const DemoRoutes = () => (
    <>
        <Route path={routes.CHAT}>
            <Demo />
        </Route>
    </>
);

const SavedRoutes = () => (
    <>
        <Route path={routes.SAVED}>
            <Saved />
        </Route>
    </>
);

const OwnMapRoutes = () => (
    <>
        <Route path={routes.OWNMAP}>
            <OwnMap />
        </Route>
    </>
);

const ProfileRoutes = () => (
    <>
        <Route path={routes.PROFILE}>
            <Profile />
        </Route>
    </>
);

const RegRoutes = () => (
    <>
        <Route path={routes.REGISTRATION}>
            <Reg />
        </Route>
    </>
);

const SettingsRoutes = () => (
    <>
        <Route path={routes.SETTINGS}>
            <Settings />
        </Route>
    </>
);

const LoginRoutes = () => (
    <>
        <Route path={routes.LOGIN}>
            <Login />
        </Route>
    </>
);

const MapPageRoutes = () => (
    <>
        <Route path={routes.MAP}>
            <MapPage />
        </Route>
    </>
);

const IntroRoutes = () => (
    <>
        <Route path={routes.INTRO}>
            <Intro />
        </Route>
    </>
);

const HomeRoutes = () => (
    <>
        <Route path={routes.HOME}>
            <Home />
        </Route>
    </>
);

export const App = () => {
    const [headerBgColor, setHeaderBgColor] = useState<string>('#39494C');
    const [mainBgColor, setMainBgColor] = useState<string>('#fcfafa');
    const [footerBgColor, setFooterBgColor] = useState<string>('#39494C');

    const funcChange = [setHeaderBgColor, setMainBgColor, setFooterBgColor];

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

        const helpKeynodes = [{ id: componentColor, type: ScType.NodeConstNoRole }];

        const colorAlias = '_color';
        const componentAlias = '_component';

        const keynodes = await client.resolveKeynodes(baseKeynodes);
        const hKeynodes = await client.resolveKeynodes(helpKeynodes);

        for (var i = 0; i < baseKeynodes.length; i++) {
            const template = new ScTemplate();
            template.triple(keynodes[baseKeynodes[i].id], ScType.EdgeAccessVarPosPerm, [
                ScType.NodeVar,
                componentAlias,
            ]);
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

    const mainStyles = {
        background: mainBgColor,
    };

    return (
        <Content style={mainStyles}>
            <DemoRoutes />
            <MapPageRoutes />
            <LoginRoutes />
            <RegRoutes />
            <HomeRoutes />
            <SavedRoutes />
            <ProfileRoutes />
            <SettingsRoutes />
            <IntroRoutes />
            <OwnMapRoutes />
        </Content>
    );
};
