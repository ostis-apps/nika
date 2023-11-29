import * as React from 'react';
import type { SelectProps } from '../select';
import Select from '../select';
interface MiniOrMiddleSelectInterface extends React.FC<SelectProps> {
    Option: typeof Select.Option;
}
declare const MiniSelect: MiniOrMiddleSelectInterface;
declare const MiddleSelect: MiniOrMiddleSelectInterface;
export { MiniSelect, MiddleSelect };
