import type * as React from 'react';
import Group from './Group';
import type { InputProps, InputRef } from './Input';
import Password from './Password';
import Search from './Search';
import TextArea from './TextArea';
export { GroupProps } from './Group';
export { InputProps, InputRef } from './Input';
export { PasswordProps } from './Password';
export { SearchProps } from './Search';
export { TextAreaProps } from './TextArea';
interface CompoundedComponent extends React.ForwardRefExoticComponent<InputProps & React.RefAttributes<InputRef>> {
    Group: typeof Group;
    Search: typeof Search;
    TextArea: typeof TextArea;
    Password: typeof Password;
}
declare const Input: CompoundedComponent;
export default Input;
