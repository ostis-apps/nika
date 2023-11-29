/// <reference types="react" />
import type { ColumnTitle, ColumnTitleProps, ColumnType, Key } from './interface';
export declare function getColumnKey<RecordType>(column: ColumnType<RecordType>, defaultKey: string): Key;
export declare function getColumnPos(index: number, pos?: string): string;
export declare function renderColumnTitle<RecordType>(title: ColumnTitle<RecordType>, props: ColumnTitleProps<RecordType>): import("react").ReactNode;
