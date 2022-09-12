import { configureStore } from '@reduxjs/toolkit';
import { profileSlice } from './profileSlice';
import { commonSlice } from './commonSlice';

export const store = configureStore({
    reducer: {
        [profileSlice.name]: profileSlice.reducer,
        [commonSlice.name]: commonSlice.reducer,
    },
});
