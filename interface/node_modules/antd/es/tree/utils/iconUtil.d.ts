import * as React from 'react';
import type { AntTreeNodeProps, SwitcherIcon } from '../Tree';
export default function renderSwitcherIcon(prefixCls: string, switcherIcon: SwitcherIcon, showLine: boolean | {
    showLeafIcon: boolean;
} | undefined, treeNodeProps: AntTreeNodeProps): React.ReactNode;
