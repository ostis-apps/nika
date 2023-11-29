import type { ForwardRefExoticComponent, RefAttributes } from 'react';
import type { AvatarProps } from './avatar';
import Group from './group';
export { AvatarProps } from './avatar';
export { GroupProps } from './group';
export { Group };
interface CompoundedComponent extends ForwardRefExoticComponent<AvatarProps & RefAttributes<HTMLElement>> {
    Group: typeof Group;
}
declare const Avatar: CompoundedComponent;
export default Avatar;
