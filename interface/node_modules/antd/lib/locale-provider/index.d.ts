import type { ValidateMessages } from 'rc-field-form/lib/interface';
import * as React from 'react';
import type { PickerLocale as DatePickerLocale } from '../date-picker/generatePicker';
import type { TransferLocale as TransferLocaleForEmpty } from '../empty';
import type { ModalLocale } from '../modal/locale';
import type { PaginationLocale } from '../pagination/Pagination';
import type { PopconfirmLocale } from '../popconfirm/PurePanel';
import type { TableLocale } from '../table/interface';
import type { TransferLocale } from '../transfer';
import type { UploadLocale } from '../upload/interface';
export declare const ANT_MARK = "internalMark";
export interface Locale {
    locale: string;
    Pagination?: PaginationLocale;
    DatePicker?: DatePickerLocale;
    TimePicker?: Record<string, any>;
    Calendar?: Record<string, any>;
    Table?: TableLocale;
    Modal?: ModalLocale;
    Popconfirm?: PopconfirmLocale;
    Transfer?: Partial<TransferLocale>;
    Select?: Record<string, any>;
    Upload?: UploadLocale;
    Empty?: TransferLocaleForEmpty;
    global?: Record<string, any>;
    PageHeader?: {
        back: string;
    };
    Icon?: Record<string, any>;
    Text?: {
        edit?: any;
        copy?: any;
        copied?: any;
        expand?: any;
    };
    Form?: {
        optional?: string;
        defaultValidateMessages: ValidateMessages;
    };
    Image?: {
        preview: string;
    };
}
export interface LocaleProviderProps {
    locale: Locale;
    children?: React.ReactNode;
    _ANT_MARK__?: string;
}
export default class LocaleProvider extends React.Component<LocaleProviderProps, any> {
    static defaultProps: {
        locale: {};
    };
    constructor(props: LocaleProviderProps);
    componentDidMount(): void;
    componentDidUpdate(prevProps: LocaleProviderProps): void;
    componentWillUnmount(): void;
    getMemoizedContextValue: import("memoize-one").MemoizedFn<(localeValue: Locale) => Locale & {
        exist?: boolean;
    }>;
    render(): JSX.Element;
}
