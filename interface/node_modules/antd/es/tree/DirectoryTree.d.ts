import type RcTree from 'rc-tree';
import type { BasicDataNode } from 'rc-tree';
import type { DataNode, Key } from 'rc-tree/lib/interface';
import * as React from 'react';
import type { TreeProps } from './Tree';
export declare type ExpandAction = false | 'click' | 'doubleClick';
export interface DirectoryTreeProps<T extends BasicDataNode = DataNode> extends TreeProps<T> {
    expandAction?: ExpandAction;
}
declare type DirectoryTreeCompoundedComponent = (<T extends BasicDataNode | DataNode = DataNode>(props: React.PropsWithChildren<DirectoryTreeProps<T>> & {
    ref?: React.Ref<RcTree>;
}) => React.ReactElement) & {
    defaultProps: Partial<React.PropsWithChildren<DirectoryTreeProps<any>>>;
    displayName?: string;
};
export interface DirectoryTreeState {
    expandedKeys?: Key[];
    selectedKeys?: Key[];
}
declare const ForwardDirectoryTree: DirectoryTreeCompoundedComponent;
export default ForwardDirectoryTree;
