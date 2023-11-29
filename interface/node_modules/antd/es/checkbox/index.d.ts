import type * as React from 'react';
import type { CheckboxProps } from './Checkbox';
import Group from './Group';
export { CheckboxChangeEvent, CheckboxProps } from './Checkbox';
export { CheckboxGroupProps, CheckboxOptionType } from './Group';
interface CompoundedComponent extends React.ForwardRefExoticComponent<CheckboxProps & React.RefAttributes<HTMLInputElement>> {
    Group: typeof Group;
    __ANT_CHECKBOX: boolean;
}
declare const Checkbox: CompoundedComponent;
export default Checkbox;
