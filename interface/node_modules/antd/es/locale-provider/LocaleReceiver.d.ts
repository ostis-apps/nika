import * as React from 'react';
import type { Locale } from '.';
import type { LocaleContextProps } from './context';
export declare type LocaleComponentName = Exclude<keyof Locale, 'locale'>;
export interface LocaleReceiverProps<C extends LocaleComponentName = LocaleComponentName> {
    componentName: C;
    defaultLocale?: Locale[C] | (() => Locale[C]);
    children: (locale: Locale[C], localeCode?: string, fullLocale?: object) => React.ReactNode;
}
export default class LocaleReceiver<C extends LocaleComponentName = LocaleComponentName> extends React.Component<LocaleReceiverProps<C>> {
    static defaultProps: {
        componentName: string;
    };
    static contextType: React.Context<LocaleContextProps | undefined>;
    context: LocaleContextProps;
    getLocale(): Locale[C];
    getLocaleCode(): string | undefined;
    render(): React.ReactNode;
}
export declare function useLocaleReceiver<T extends LocaleComponentName>(componentName: T, defaultLocale?: Locale[T] | Function): [Locale[T]];
