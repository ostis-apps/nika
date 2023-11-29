import * as React from 'react';
interface BreadcrumbSeparatorInterface extends React.FC<{
    children?: React.ReactNode;
}> {
    __ANT_BREADCRUMB_SEPARATOR: boolean;
}
declare const BreadcrumbSeparator: BreadcrumbSeparatorInterface;
export default BreadcrumbSeparator;
