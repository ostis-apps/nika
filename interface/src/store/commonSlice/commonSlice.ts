import { createSlice, PayloadAction } from '@reduxjs/toolkit';
import { IRootState } from '@store';

const initialState = {
    language: localStorage.getItem('language') === null ? 'ru' : `${localStorage.getItem('language')}`,
};

export const commonSlice = createSlice({
    name: 'common',
    initialState,
    reducers: {
        setLanguage: (state, action: PayloadAction<string>) => {
            localStorage.removeItem('language');
            localStorage.setItem('language', action.payload);
            state.language = action.payload;
        },
    },
});

export const selectLanguage = (state: IRootState) => state.common.language;
