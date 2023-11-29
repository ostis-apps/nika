import Col from './col';
import Row from './row';
declare function useBreakpoint(): Partial<Record<import("../_util/responsiveObserve").Breakpoint, boolean>>;
export { ColProps, ColSize } from './col';
export { RowProps } from './row';
export { Row, Col };
declare const _default: {
    useBreakpoint: typeof useBreakpoint;
};
export default _default;
