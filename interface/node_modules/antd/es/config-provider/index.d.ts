import type { ValidateMessages } from 'rc-field-form/lib/interface';
import * as React from 'react';
import type { RequiredMark } from '../form/Form';
import type { Locale } from '../locale-provider';
import type { Theme } from './context';
import { ConfigConsumer, ConfigConsumerProps, ConfigContext, CSPConfig, DirectionType } from './context';
import { RenderEmptyHandler } from './defaultRenderEmpty';
import type { SizeType } from './SizeContext';
import SizeContext from './SizeContext';
export { RenderEmptyHandler, ConfigContext, ConfigConsumer, CSPConfig, DirectionType, ConfigConsumerProps, };
export declare const configConsumerProps: string[];
export interface ConfigProviderProps {
    getTargetContainer?: () => HTMLElement;
    getPopupContainer?: (triggerNode?: HTMLElement) => HTMLElement;
    prefixCls?: string;
    iconPrefixCls?: string;
    children?: React.ReactNode;
    renderEmpty?: RenderEmptyHandler;
    csp?: CSPConfig;
    autoInsertSpaceInButton?: boolean;
    form?: {
        validateMessages?: ValidateMessages;
        requiredMark?: RequiredMark;
        colon?: boolean;
    };
    input?: {
        autoComplete?: string;
    };
    pagination?: {
        showSizeChanger?: boolean;
    };
    locale?: Locale;
    pageHeader?: {
        ghost: boolean;
    };
    componentSize?: SizeType;
    componentDisabled?: boolean;
    direction?: DirectionType;
    space?: {
        size?: SizeType | number;
    };
    virtual?: boolean;
    dropdownMatchSelectWidth?: boolean;
}
export declare const defaultPrefixCls = "ant";
export declare const defaultIconPrefixCls = "anticon";
declare function getGlobalIconPrefixCls(): string;
declare const setGlobalConfig: ({ prefixCls, iconPrefixCls, theme, }: Pick<ConfigProviderProps, "prefixCls" | "iconPrefixCls"> & {
    theme?: Theme | undefined;
}) => void;
export declare const globalConfig: () => {
    getPrefixCls: (suffixCls?: string, customizePrefixCls?: string) => string;
    getIconPrefixCls: typeof getGlobalIconPrefixCls;
    getRootPrefixCls: (rootPrefixCls?: string, customizePrefixCls?: string) => string;
};
declare const ConfigProvider: React.FC<ConfigProviderProps> & {
    ConfigContext: typeof ConfigContext;
    SizeContext: typeof SizeContext;
    config: typeof setGlobalConfig;
};
export default ConfigProvider;
