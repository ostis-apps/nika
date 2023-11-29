import type { InputNumberProps as RcInputNumberProps } from 'rc-input-number';
import * as React from 'react';
import type { SizeType } from '../config-provider/SizeContext';
import type { InputStatus } from '../_util/statusUtils';
declare type ValueType = string | number;
export interface InputNumberProps<T extends ValueType = ValueType> extends Omit<RcInputNumberProps<T>, 'prefix' | 'size' | 'controls'> {
    prefixCls?: string;
    addonBefore?: React.ReactNode;
    addonAfter?: React.ReactNode;
    prefix?: React.ReactNode;
    size?: SizeType;
    disabled?: boolean;
    bordered?: boolean;
    status?: InputStatus;
    controls?: boolean | {
        upIcon?: React.ReactNode;
        downIcon?: React.ReactNode;
    };
}
declare const _default: (<T extends ValueType = ValueType>(props: InputNumberProps<T> & {
    children?: React.ReactNode;
} & {
    ref?: React.Ref<HTMLInputElement> | undefined;
}) => React.ReactElement) & {
    displayName?: string | undefined;
};
export default _default;
