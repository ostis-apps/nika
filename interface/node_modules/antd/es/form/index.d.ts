import { Rule, RuleObject, RuleRender } from 'rc-field-form/lib/interface';
import { FormProvider } from './context';
import ErrorList, { ErrorListProps } from './ErrorList';
import InternalForm, { FormInstance, FormProps, useForm, useWatch } from './Form';
import Item, { FormItemProps } from './FormItem';
import List, { FormListFieldData, FormListOperation, FormListProps } from './FormList';
import useFormInstance from './hooks/useFormInstance';
declare type InternalFormType = typeof InternalForm;
interface FormInterface extends InternalFormType {
    useForm: typeof useForm;
    useFormInstance: typeof useFormInstance;
    useWatch: typeof useWatch;
    Item: typeof Item;
    List: typeof List;
    ErrorList: typeof ErrorList;
    Provider: typeof FormProvider;
    /** @deprecated Only for warning usage. Do not use. */
    create: () => void;
}
declare const Form: FormInterface;
export { FormInstance, FormProps, FormItemProps, ErrorListProps, Rule, RuleObject, RuleRender, FormListProps, FormListFieldData, FormListOperation, };
export default Form;
