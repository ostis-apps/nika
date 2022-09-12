import { createContext } from 'react';
import { IToastContext } from './model';

const defaultContext: IToastContext = {
    toasts: [],
    addToast: () => void 0,
    removeToast: () => void 0,
};

export const ToastContext = createContext<IToastContext>(defaultContext);
