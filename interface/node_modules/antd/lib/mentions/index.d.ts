import type { MentionsProps as RcMentionsProps } from 'rc-mentions/lib/Mentions';
import * as React from 'react';
import type { InputStatus } from '../_util/statusUtils';
export declare const Option: React.FC<import("rc-mentions/lib/Option").OptionProps>;
export declare type MentionPlacement = 'top' | 'bottom';
export interface OptionProps {
    value: string;
    children: React.ReactNode;
    [key: string]: any;
}
export interface MentionProps extends RcMentionsProps {
    loading?: boolean;
    status?: InputStatus;
}
export interface MentionState {
    focused: boolean;
}
interface MentionsConfig {
    prefix?: string | string[];
    split?: string;
}
interface MentionsEntity {
    prefix: string;
    value: string;
}
interface CompoundedComponent extends React.ForwardRefExoticComponent<MentionProps & React.RefAttributes<HTMLElement>> {
    Option: typeof Option;
    getMentions: (value: string, config?: MentionsConfig) => MentionsEntity[];
}
declare const Mentions: CompoundedComponent;
export default Mentions;
