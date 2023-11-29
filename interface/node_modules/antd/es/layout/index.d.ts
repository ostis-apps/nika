import InternalLayout, { Content, Footer, Header } from './layout';
import Sider from './Sider';
export { BasicProps as LayoutProps } from './layout';
export { SiderProps } from './Sider';
declare type InternalLayoutType = typeof InternalLayout;
interface LayoutType extends InternalLayoutType {
    Header: typeof Header;
    Footer: typeof Footer;
    Content: typeof Content;
    Sider: typeof Sider;
}
declare const Layout: LayoutType;
export default Layout;
