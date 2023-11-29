import * as React from 'react';
import type { AvatarProps } from '../avatar';
import type { BreadcrumbProps } from '../breadcrumb';
import Breadcrumb from '../breadcrumb';
import type { TagType } from '../tag';
export interface PageHeaderProps {
    backIcon?: React.ReactNode;
    prefixCls?: string;
    title?: React.ReactNode;
    subTitle?: React.ReactNode;
    style?: React.CSSProperties;
    breadcrumb?: BreadcrumbProps | React.ReactElement<typeof Breadcrumb>;
    breadcrumbRender?: (props: PageHeaderProps, defaultDom: React.ReactNode) => React.ReactNode;
    tags?: React.ReactElement<TagType> | React.ReactElement<TagType>[];
    footer?: React.ReactNode;
    extra?: React.ReactNode;
    avatar?: AvatarProps;
    onBack?: (e?: React.MouseEvent<HTMLDivElement>) => void;
    className?: string;
    ghost?: boolean;
    children?: React.ReactNode;
}
declare const PageHeader: React.FC<PageHeaderProps>;
export default PageHeader;
