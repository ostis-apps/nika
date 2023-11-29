import * as React from 'react';
import type { MenuProps } from '.';
export interface OverrideContextProps {
    prefixCls?: string;
    expandIcon?: React.ReactNode;
    mode?: MenuProps['mode'];
    selectable?: boolean;
    validator?: (menuProps: Pick<MenuProps, 'mode'>) => void;
    onClick?: () => void;
}
/** @private Internal Usage. Only used for Dropdown component. Do not use this in your production. */
declare const OverrideContext: React.Context<OverrideContextProps | null>;
/** @private Internal Usage. Only used for Dropdown component. Do not use this in your production. */
export declare const OverrideProvider: ({ children, ...restProps }: OverrideContextProps & {
    children: React.ReactNode;
}) => JSX.Element;
export default OverrideContext;
