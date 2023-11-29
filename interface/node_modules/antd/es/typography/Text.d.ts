import * as React from 'react';
import type { BlockProps, EllipsisConfig } from './Base';
export interface TextProps extends BlockProps {
    ellipsis?: boolean | Omit<EllipsisConfig, 'expandable' | 'rows' | 'onExpand'>;
    onClick?: (e?: React.MouseEvent<HTMLDivElement>) => void;
}
declare const _default: React.ForwardRefExoticComponent<TextProps & React.RefAttributes<HTMLSpanElement>>;
export default _default;
