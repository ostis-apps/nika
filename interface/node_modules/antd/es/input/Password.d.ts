import * as React from 'react';
import type { InputProps, InputRef } from './Input';
export interface PasswordProps extends InputProps {
    readonly inputPrefixCls?: string;
    readonly action?: string;
    visibilityToggle?: boolean;
    iconRender?: (visible: boolean) => React.ReactNode;
}
declare const Password: React.ForwardRefExoticComponent<PasswordProps & React.RefAttributes<InputRef>>;
export default Password;
