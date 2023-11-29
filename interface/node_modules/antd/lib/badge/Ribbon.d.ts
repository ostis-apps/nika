import * as React from 'react';
import type { PresetColorType } from '../_util/colors';
import type { LiteralUnion } from '../_util/type';
declare type RibbonPlacement = 'start' | 'end';
export interface RibbonProps {
    className?: string;
    prefixCls?: string;
    style?: React.CSSProperties;
    text?: React.ReactNode;
    color?: LiteralUnion<PresetColorType, string>;
    children?: React.ReactNode;
    placement?: RibbonPlacement;
}
declare const Ribbon: React.FC<RibbonProps>;
export default Ribbon;
