import type { BaseOptionType, DefaultOptionType, FieldNames, MultipleCascaderProps as RcMultipleCascaderProps, SingleCascaderProps as RcSingleCascaderProps } from 'rc-cascader';
import * as React from 'react';
import type { SizeType } from '../config-provider/SizeContext';
import type { SelectCommonPlacement } from '../_util/motion';
import type { InputStatus } from '../_util/statusUtils';
export { BaseOptionType, DefaultOptionType };
export declare type FieldNamesType = FieldNames;
export declare type FilledFieldNamesType = Required<FieldNamesType>;
declare type SingleCascaderProps = Omit<RcSingleCascaderProps, 'checkable' | 'options'> & {
    multiple?: false;
};
declare type MultipleCascaderProps = Omit<RcMultipleCascaderProps, 'checkable' | 'options'> & {
    multiple: true;
};
declare type UnionCascaderProps = SingleCascaderProps | MultipleCascaderProps;
export declare type CascaderProps<DataNodeType> = UnionCascaderProps & {
    multiple?: boolean;
    size?: SizeType;
    disabled?: boolean;
    bordered?: boolean;
    placement?: SelectCommonPlacement;
    suffixIcon?: React.ReactNode;
    options?: DataNodeType[];
    status?: InputStatus;
};
export interface CascaderRef {
    focus: () => void;
    blur: () => void;
}
declare const Cascader: (<OptionType extends DefaultOptionType | BaseOptionType = DefaultOptionType>(props: React.PropsWithChildren<CascaderProps<OptionType>> & {
    ref?: React.Ref<CascaderRef> | undefined;
}) => React.ReactElement) & {
    displayName: string;
    SHOW_PARENT: "SHOW_PARENT";
    SHOW_CHILD: "SHOW_CHILD";
};
export default Cascader;
