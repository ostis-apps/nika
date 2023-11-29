import type { GenerateConfig } from 'rc-picker/lib/generate/index';
import type { Locale as RcPickerLocale, PickerMode } from 'rc-picker/lib/interface';
import type { SharedTimeProps } from 'rc-picker/lib/panels/TimePanel';
import type { PickerBaseProps as RCPickerBaseProps, PickerDateProps as RCPickerDateProps, PickerTimeProps as RCPickerTimeProps } from 'rc-picker/lib/Picker';
import type { RangePickerBaseProps as RCRangePickerBaseProps, RangePickerDateProps as RCRangePickerDateProps, RangePickerTimeProps as RCRangePickerTimeProps } from 'rc-picker/lib/RangePicker';
import type { SizeType } from '../../config-provider/SizeContext';
import type { TimePickerLocale } from '../../time-picker';
import type { InputStatus } from '../../_util/statusUtils';
import PickerButton from '../PickerButton';
import PickerTag from '../PickerTag';
export declare const Components: {
    button: typeof PickerButton;
    rangeItem: typeof PickerTag;
};
export declare function getTimeProps<DateType, DisabledTime>(props: {
    format?: string;
    picker?: PickerMode;
} & Omit<SharedTimeProps<DateType>, 'disabledTime'> & {
    disabledTime?: DisabledTime;
}): {
    format?: string | undefined;
    picker?: PickerMode | undefined;
    defaultValue?: DateType | undefined;
    showNow?: boolean | undefined;
    showHour?: boolean | undefined;
    showMinute?: boolean | undefined;
    showSecond?: boolean | undefined;
    use12Hours?: boolean | undefined;
    hourStep?: number | undefined;
    minuteStep?: number | undefined;
    secondStep?: number | undefined;
    hideDisabledOptions?: boolean | undefined;
    disabledHours?: (() => number[]) | undefined;
    disabledMinutes?: ((hour: number) => number[]) | undefined;
    disabledSeconds?: ((hour: number, minute: number) => number[]) | undefined;
    disabledTime?: DisabledTime | undefined;
} | {
    showTime: {
        format?: string | undefined;
        picker?: PickerMode | undefined;
        defaultValue?: DateType | undefined;
        showNow?: boolean | undefined;
        showHour?: boolean | undefined;
        showMinute?: boolean | undefined;
        showSecond?: boolean | undefined;
        use12Hours?: boolean | undefined;
        hourStep?: number | undefined;
        minuteStep?: number | undefined;
        secondStep?: number | undefined;
        hideDisabledOptions?: boolean | undefined;
        disabledHours?: (() => number[]) | undefined;
        disabledMinutes?: ((hour: number) => number[]) | undefined;
        disabledSeconds?: ((hour: number, minute: number) => number[]) | undefined;
        disabledTime?: DisabledTime | undefined;
    };
};
declare const DataPickerPlacements: ["bottomLeft", "bottomRight", "topLeft", "topRight"];
declare type DataPickerPlacement = typeof DataPickerPlacements[number];
declare type InjectDefaultProps<Props> = Omit<Props, 'locale' | 'generateConfig' | 'hideHeader' | 'components'> & {
    locale?: PickerLocale;
    size?: SizeType;
    placement?: DataPickerPlacement;
    bordered?: boolean;
    status?: InputStatus;
};
export declare type PickerLocale = {
    lang: RcPickerLocale & AdditionalPickerLocaleLangProps;
    timePickerLocale: TimePickerLocale;
} & AdditionalPickerLocaleProps;
export declare type AdditionalPickerLocaleProps = {
    dateFormat?: string;
    dateTimeFormat?: string;
    weekFormat?: string;
    monthFormat?: string;
};
export declare type AdditionalPickerLocaleLangProps = {
    placeholder: string;
    yearPlaceholder?: string;
    quarterPlaceholder?: string;
    monthPlaceholder?: string;
    weekPlaceholder?: string;
    rangeYearPlaceholder?: [string, string];
    rangeQuarterPlaceholder?: [string, string];
    rangeMonthPlaceholder?: [string, string];
    rangeWeekPlaceholder?: [string, string];
    rangePlaceholder?: [string, string];
};
export declare type PickerBaseProps<DateType> = InjectDefaultProps<RCPickerBaseProps<DateType>>;
export declare type PickerDateProps<DateType> = InjectDefaultProps<RCPickerDateProps<DateType>>;
export declare type PickerTimeProps<DateType> = InjectDefaultProps<RCPickerTimeProps<DateType>>;
export declare type PickerProps<DateType> = PickerBaseProps<DateType> | PickerDateProps<DateType> | PickerTimeProps<DateType>;
export declare type RangePickerBaseProps<DateType> = InjectDefaultProps<RCRangePickerBaseProps<DateType>>;
export declare type RangePickerDateProps<DateType> = InjectDefaultProps<RCRangePickerDateProps<DateType>>;
export declare type RangePickerTimeProps<DateType> = InjectDefaultProps<RCRangePickerTimeProps<DateType>>;
export declare type RangePickerProps<DateType> = RangePickerBaseProps<DateType> | RangePickerDateProps<DateType> | RangePickerTimeProps<DateType>;
declare function generatePicker<DateType>(generateConfig: GenerateConfig<DateType>): import("./interface").PickerComponentClass<PickerProps<DateType> & {
    status?: "" | "warning" | "error" | undefined;
}, unknown> & {
    WeekPicker: import("./interface").PickerComponentClass<Omit<PickerDateProps<DateType>, "picker">, unknown>;
    MonthPicker: import("./interface").PickerComponentClass<Omit<PickerDateProps<DateType>, "picker">, unknown>;
    YearPicker: import("./interface").PickerComponentClass<Omit<PickerDateProps<DateType>, "picker">, unknown>;
    RangePicker: import("./interface").PickerComponentClass<RangePickerProps<DateType>, unknown>;
    TimePicker: import("./interface").PickerComponentClass<Omit<PickerTimeProps<DateType>, "picker">, unknown>;
    QuarterPicker: import("./interface").PickerComponentClass<Omit<PickerTimeProps<DateType>, "picker">, unknown>;
};
export default generatePicker;
