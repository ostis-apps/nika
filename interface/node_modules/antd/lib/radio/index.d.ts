import type * as React from 'react';
import Group from './group';
import type { RadioProps } from './interface';
import Button from './radioButton';
export { RadioChangeEvent, RadioChangeEventTarget, RadioGroupButtonStyle, RadioGroupContextProps, RadioGroupOptionType, RadioGroupProps, RadioProps, } from './interface';
export { Button, Group };
interface CompoundedComponent extends React.ForwardRefExoticComponent<RadioProps & React.RefAttributes<HTMLElement>> {
    Group: typeof Group;
    Button: typeof Button;
    __ANT_RADIO: boolean;
}
declare const Radio: CompoundedComponent;
export default Radio;
