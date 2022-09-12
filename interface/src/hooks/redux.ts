import { Dispatch, IRootState } from '@store';
import { createSelectorHook, useDispatch as useReduxDispatch } from 'react-redux';

export const useDispatch = () => useReduxDispatch<Dispatch>();
export const useSelector = createSelectorHook<IRootState>();
