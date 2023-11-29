import * as React from 'react';
import type { AbstractTooltipProps } from '../tooltip';
import type { RenderFunction } from '../_util/getRenderPropValue';
export interface PopoverProps extends AbstractTooltipProps {
    title?: React.ReactNode | RenderFunction;
    content?: React.ReactNode | RenderFunction;
    /** @private Used For Popconfirm. Safe to remove. */
    _overlay?: React.ReactNode;
}
declare const Popover: React.ForwardRefExoticComponent<PopoverProps & React.RefAttributes<unknown>>;
export default Popover;
