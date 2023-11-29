import * as React from 'react';
import type { BlockProps } from './Base';
export interface ParagraphProps extends BlockProps {
    onClick?: (e?: React.MouseEvent<HTMLDivElement>) => void;
}
declare const _default: React.ForwardRefExoticComponent<ParagraphProps & React.RefAttributes<HTMLDivElement>>;
export default _default;
