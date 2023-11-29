import type { SkeletonElementProps } from './Element';
export interface SkeletonImageProps extends Omit<SkeletonElementProps, 'size' | 'shape'> {
}
declare const SkeletonImage: (props: SkeletonImageProps) => JSX.Element;
export default SkeletonImage;
