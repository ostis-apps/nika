import * as React from 'react';
import type { BlockProps } from './Base';
export interface LinkProps extends BlockProps, Omit<React.AnchorHTMLAttributes<HTMLAnchorElement>, 'type'> {
    ellipsis?: boolean;
}
declare const _default: React.ForwardRefExoticComponent<LinkProps & React.RefAttributes<HTMLElement>>;
export default _default;
