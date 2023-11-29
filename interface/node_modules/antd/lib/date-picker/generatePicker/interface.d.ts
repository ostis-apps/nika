import type { Component, ComponentClass, ForwardedRef } from 'react';
import type { PickerProps, RangePickerProps } from '.';
export interface CommonPickerMethods {
    focus: () => void;
    blur: () => void;
}
export interface PickerComponentClass<P = {}, S = unknown> extends ComponentClass<P, S> {
    new (...args: ConstructorParameters<ComponentClass<P, S>>): InstanceType<ComponentClass<P, S>> & CommonPickerMethods;
}
export declare type PickerRef<P> = ForwardedRef<Component<P> & CommonPickerMethods>;
export declare type DatePickRef<DateType> = PickerRef<PickerProps<DateType>>;
export declare type RangePickerRef<DateType> = PickerRef<RangePickerProps<DateType>>;
