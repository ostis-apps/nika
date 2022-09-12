import { lazy } from "react";
import { Route, Redirect } from "react-router-dom";
import { loadingComponent } from '@components/LoadingComponent';
import { routes } from '@constants';

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
        <Route path={routes.MAIN}>
            <Demo />
        </Route>
        <Redirect to={routes.MAIN} />
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
    return (
        <Layout>
            <Header>
                <HeaderPanel />
            </Header>
            <Content>
                <DemoRoutes />
                <AboutRoutes />
            </Content>
            <Footer>
                <FooterPanel />
            </Footer>
        </Layout>
    );
};
