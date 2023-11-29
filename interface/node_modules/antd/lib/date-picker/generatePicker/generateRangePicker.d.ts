import type { GenerateConfig } from 'rc-picker/lib/generate/index';
import type { RangePickerProps } from '.';
import type { PickerComponentClass } from './interface';
export default function generateRangePicker<DateType>(generateConfig: GenerateConfig<DateType>): PickerComponentClass<RangePickerProps<DateType>>;
