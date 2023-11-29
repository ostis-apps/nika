import * as React from 'react';
import type { DropdownProps } from '../dropdown/dropdown';
export interface BreadcrumbItemProps {
    prefixCls?: string;
    separator?: React.ReactNode;
    href?: string;
    overlay?: DropdownProps['overlay'];
    dropdownProps?: DropdownProps;
    onClick?: React.MouseEventHandler<HTMLAnchorElement | HTMLSpanElement>;
    className?: string;
    children?: React.ReactNode;
}
interface BreadcrumbItemInterface extends React.FC<BreadcrumbItemProps> {
    __ANT_BREADCRUMB_ITEM: boolean;
}
declare const BreadcrumbItem: BreadcrumbItemInterface;
export default BreadcrumbItem;
