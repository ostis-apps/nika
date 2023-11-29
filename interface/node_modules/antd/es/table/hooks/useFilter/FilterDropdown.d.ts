import * as React from 'react';
import type { FilterState } from '.';
import type { ColumnType, FilterSearchType, GetPopupContainer, Key, TableLocale } from '../../interface';
export interface FilterDropdownProps<RecordType> {
    tablePrefixCls: string;
    prefixCls: string;
    dropdownPrefixCls: string;
    column: ColumnType<RecordType>;
    filterState?: FilterState<RecordType>;
    filterMultiple: boolean;
    filterMode?: 'menu' | 'tree';
    filterSearch?: FilterSearchType;
    columnKey: Key;
    children: React.ReactNode;
    triggerFilter: (filterState: FilterState<RecordType>) => void;
    locale: TableLocale;
    getPopupContainer?: GetPopupContainer;
    filterResetToDefaultFilteredValue?: boolean;
}
declare function FilterDropdown<RecordType>(props: FilterDropdownProps<RecordType>): JSX.Element;
export default FilterDropdown;
