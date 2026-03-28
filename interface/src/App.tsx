import { lazy, useEffect, useState} from "react";
import { loadingComponent } from '@components/LoadingComponent';
import { routes } from '@constants';
import { BrowserRouter as Router, Switch, Route, Redirect } from 'react-router-dom';
import 'antd/dist/antd.css';
import './assets/main.css';

import { Layout } from 'antd';

import { HeaderPanel } from "@components/Header";
import { FooterPanel } from "@components/Footer";
import { fetchColorValue } from "@api/sc/agents/fetchColorValueAgent";
import { AuthPage } from "@pages/Auth/AuthPage";

const { Header, Content, Footer } = Layout;

const Demo = loadingComponent(lazy(() => import('@pages/Demo')));
const About = loadingComponent(lazy(() => import('@pages/About')));
const GoogleCallback = loadingComponent(lazy(() => import('@pages/Auth/Callback/Google')));

export const App = () => {
    const [headerBgColor, setHeaderBgColor] = useState<string>('#39494C');
    const [mainBgColor, setMainBgColor] = useState<string>('#fcfafa');
    const [footerBgColor, setFooterBgColor] = useState<string>('#39494C');
    
    const funcChange = [setHeaderBgColor, setMainBgColor, setFooterBgColor]

    useEffect(() => {
        fetchColorValue(funcChange);
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
        <Router>
      <Layout>
        <Header style={headerStyles}>
          <HeaderPanel />
        </Header>
        <Content style={mainStyles}>
          <Switch> 
            <Route exact path={routes.MAIN} component={Demo} />
            <Route path={routes.ABOUT} component={About} />
            <Route exact path={routes.GOOGLE_CALLBACK} component={GoogleCallback} />
            <Route path={routes.AUTH} component={AuthPage} />
            <Route path="*">
              <Redirect to={routes.MAIN} />
            </Route>
          </Switch>
        </Content>
        <Footer style={footerStyles}>
          <FooterPanel />
        </Footer>
      </Layout>
    </Router>
  );
};
