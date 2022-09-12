import { useContext } from 'react';
import { ToastContext } from './constants';

export const useToast = () => useContext(ToastContext);
