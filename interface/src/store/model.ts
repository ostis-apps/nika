import { store } from './store';

export type IRootState = ReturnType<typeof store.getState>;
export type Dispatch = typeof store.dispatch;
