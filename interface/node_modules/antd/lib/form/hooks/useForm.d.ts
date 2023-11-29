import type { FormInstance as RcFormInstance } from 'rc-field-form';
import * as React from 'react';
import type { InternalNamePath, NamePath, ScrollOptions } from '../interface';
export interface FormInstance<Values = any> extends RcFormInstance<Values> {
    scrollToField: (name: NamePath, options?: ScrollOptions) => void;
    /** This is an internal usage. Do not use in your prod */
    __INTERNAL__: {
        /** No! Do not use this in your code! */
        name?: string;
        /** No! Do not use this in your code! */
        itemRef: (name: InternalNamePath) => (node: React.ReactElement) => void;
    };
    getFieldInstance: (name: NamePath) => any;
}
export default function useForm<Values = any>(form?: FormInstance<Values>): [FormInstance<Values>];
