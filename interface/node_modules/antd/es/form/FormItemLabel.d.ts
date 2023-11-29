import * as React from 'react';
import type { ColProps } from '../grid/col';
import type { TooltipProps } from '../tooltip';
import type { RequiredMark } from './Form';
import type { FormLabelAlign } from './interface';
export declare type WrapperTooltipProps = TooltipProps & {
    icon?: React.ReactElement;
};
export declare type LabelTooltipType = WrapperTooltipProps | React.ReactNode;
export interface FormItemLabelProps {
    colon?: boolean;
    htmlFor?: string;
    label?: React.ReactNode;
    labelAlign?: FormLabelAlign;
    labelCol?: ColProps;
    requiredMark?: RequiredMark;
    tooltip?: LabelTooltipType;
}
declare const FormItemLabel: React.FC<FormItemLabelProps & {
    required?: boolean;
    prefixCls: string;
}>;
export default FormItemLabel;
