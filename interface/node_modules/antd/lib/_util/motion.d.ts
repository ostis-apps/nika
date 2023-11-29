import type { CSSMotionProps } from 'rc-motion';
declare const collapseMotion: CSSMotionProps;
declare const SelectPlacements: ["bottomLeft", "bottomRight", "topLeft", "topRight"];
export declare type SelectCommonPlacement = typeof SelectPlacements[number];
declare const getTransitionDirection: (placement: SelectCommonPlacement | undefined) => "slide-down" | "slide-up";
declare const getTransitionName: (rootPrefixCls: string, motion: string, transitionName?: string) => string;
export { getTransitionName, getTransitionDirection };
export default collapseMotion;
