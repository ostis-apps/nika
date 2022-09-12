import { ReactElement } from 'react';

interface IComponentProps {
    [x: string]: any;
    onClose?: () => void;
}

export type TToastComponent = ReactElement<IComponentProps, string | React.JSXElementConstructor<any>>;

export interface IToastParams {
    id: string;
    duration: number | 'infinity';
}

export type TAddToastParams = Partial<IToastParams>;

export interface IToast {
    params: IToastParams;
    component: TToastComponent;
}

export type TAddToast = (component: TToastComponent, params?: TAddToastParams) => void;

export interface IToastContext {
    toasts: IToast[];
    addToast: TAddToast;
    removeToast: (id: string) => void;
}
