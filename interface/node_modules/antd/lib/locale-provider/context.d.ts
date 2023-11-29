/// <reference types="react" />
import type { Locale } from '.';
export declare type LocaleContextProps = Partial<Locale> & {
    exist?: boolean;
};
declare const LocaleContext: import("react").Context<LocaleContextProps | undefined>;
export default LocaleContext;
