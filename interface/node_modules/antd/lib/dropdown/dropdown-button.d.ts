import * as React from 'react';
import type { ButtonProps } from '../button';
import type { ButtonHTMLType } from '../button/button';
import type { ButtonGroupProps } from '../button/button-group';
import type { DropdownProps } from './dropdown';
export declare type DropdownButtonType = 'default' | 'primary' | 'ghost' | 'dashed' | 'link' | 'text';
export interface DropdownButtonProps extends ButtonGroupProps, DropdownProps {
    type?: DropdownButtonType;
    htmlType?: ButtonHTMLType;
    disabled?: boolean;
    loading?: ButtonProps['loading'];
    onClick?: React.MouseEventHandler<HTMLButtonElement>;
    icon?: React.ReactNode;
    href?: string;
    children?: React.ReactNode;
    title?: string;
    buttonsRender?: (buttons: React.ReactNode[]) => React.ReactNode[];
}
interface DropdownButtonInterface extends React.FC<DropdownButtonProps> {
    __ANT_BUTTON: boolean;
}
declare const DropdownButton: DropdownButtonInterface;
export default DropdownButton;
