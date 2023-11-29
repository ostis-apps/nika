import * as React from 'react';
import type { FilterSearchType, TableLocale } from '../../interface';
interface FilterSearchProps {
    value: string;
    onChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
    filterSearch: FilterSearchType;
    tablePrefixCls: string;
    locale: TableLocale;
}
declare const FilterSearch: React.FC<FilterSearchProps>;
export default FilterSearch;
