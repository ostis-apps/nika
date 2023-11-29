import * as React from 'react';
import type { AvatarProps } from './Avatar';
import type { SkeletonParagraphProps } from './Paragraph';
import type { SkeletonTitleProps } from './Title';
interface SkeletonAvatarProps extends Omit<AvatarProps, 'active'> {
}
export interface SkeletonProps {
    active?: boolean;
    loading?: boolean;
    prefixCls?: string;
    className?: string;
    style?: React.CSSProperties;
    children?: React.ReactNode;
    avatar?: SkeletonAvatarProps | boolean;
    title?: SkeletonTitleProps | boolean;
    paragraph?: SkeletonParagraphProps | boolean;
    round?: boolean;
}
declare const Skeleton: {
    (props: SkeletonProps): JSX.Element | null;
    defaultProps: {
        avatar: boolean;
        title: boolean;
        paragraph: boolean;
    };
    Button: {
        (props: import("./Button").SkeletonButtonProps): JSX.Element;
        defaultProps: {
            size: string;
        };
    };
    Avatar: {
        (props: AvatarProps): JSX.Element;
        defaultProps: {
            size: string;
            shape: string;
        };
    };
    Input: {
        (props: import("./Input").SkeletonInputProps): JSX.Element;
        defaultProps: {
            size: string;
        };
    };
    Image: (props: import("./Image").SkeletonImageProps) => JSX.Element;
    Node: React.FC<import("./Node").SkeletonNodeProps>;
};
export default Skeleton;
