import * as React from 'react';
import type { ConfigConsumerProps } from '../config-provider';
export declare type AnchorContainer = HTMLElement | Window;
export interface AnchorProps {
    prefixCls?: string;
    className?: string;
    style?: React.CSSProperties;
    children?: React.ReactNode;
    offsetTop?: number;
    bounds?: number;
    affix?: boolean;
    showInkInFixed?: boolean;
    getContainer?: () => AnchorContainer;
    /** Return customize highlight anchor */
    getCurrentAnchor?: (activeLink: string) => string;
    onClick?: (e: React.MouseEvent<HTMLElement>, link: {
        title: React.ReactNode;
        href: string;
    }) => void;
    /** Scroll to target offset value, if none, it's offsetTop prop value or 0. */
    targetOffset?: number;
    /** Listening event when scrolling change active link */
    onChange?: (currentActiveLink: string) => void;
}
interface InternalAnchorProps extends AnchorProps {
    anchorPrefixCls: string;
}
export interface AnchorState {
    activeLink: null | string;
}
export interface AnchorDefaultProps extends AnchorProps {
    prefixCls: string;
    affix: boolean;
    showInkInFixed: boolean;
    getContainer: () => AnchorContainer;
}
export interface AntAnchor {
    registerLink: (link: string) => void;
    unregisterLink: (link: string) => void;
    activeLink: string | null;
    scrollTo: (link: string) => void;
    onClick?: (e: React.MouseEvent<HTMLElement>, link: {
        title: React.ReactNode;
        href: string;
    }) => void;
}
declare class Anchor extends React.Component<InternalAnchorProps, AnchorState, ConfigConsumerProps> {
    static defaultProps: {
        affix: boolean;
        showInkInFixed: boolean;
    };
    static contextType: React.Context<ConfigConsumerProps>;
    state: {
        activeLink: null;
    };
    context: ConfigConsumerProps;
    private wrapperRef;
    private inkNode;
    private scrollContainer;
    private links;
    private scrollEvent;
    private animating;
    private prefixCls?;
    registerLink: (link: string) => void;
    unregisterLink: (link: string) => void;
    getContainer: () => AnchorContainer;
    componentDidMount(): void;
    componentDidUpdate(): void;
    componentWillUnmount(): void;
    getCurrentAnchor(offsetTop?: number, bounds?: number): string;
    handleScrollTo: (link: string) => void;
    saveInkNode: (node: HTMLSpanElement) => void;
    setCurrentActiveLink: (link: string) => void;
    handleScroll: () => void;
    updateInk: () => void;
    getMemoizedContextValue: import("memoize-one").MemoizedFn<(link: AntAnchor['activeLink'], onClickFn: AnchorProps['onClick']) => AntAnchor>;
    render(): JSX.Element;
}
export declare type InternalAnchorClass = Anchor;
declare const AnchorFC: React.ForwardRefExoticComponent<AnchorProps & React.RefAttributes<Anchor>>;
export default AnchorFC;
