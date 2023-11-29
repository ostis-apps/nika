import type { GenerateConfig } from 'rc-picker/lib/generate/index';
import type { PickerDateProps, PickerProps, PickerTimeProps } from '.';
import type { PickerComponentClass } from './interface';
export default function generatePicker<DateType>(generateConfig: GenerateConfig<DateType>): {
    DatePicker: PickerComponentClass<PickerProps<DateType> & {
        status?: "" | "warning" | "error" | undefined;
    }, unknown>;
    WeekPicker: PickerComponentClass<Omit<PickerDateProps<DateType>, "picker">, unknown>;
    MonthPicker: PickerComponentClass<Omit<PickerDateProps<DateType>, "picker">, unknown>;
    YearPicker: PickerComponentClass<Omit<PickerDateProps<DateType>, "picker">, unknown>;
    TimePicker: PickerComponentClass<Omit<PickerTimeProps<DateType>, "picker">, unknown>;
    QuarterPicker: PickerComponentClass<Omit<PickerTimeProps<DateType>, "picker">, unknown>;
};
