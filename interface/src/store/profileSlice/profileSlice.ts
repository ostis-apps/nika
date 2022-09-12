import { createSlice, PayloadAction } from '@reduxjs/toolkit';

type Role = 'expert' | 'student' | null;

interface User {
    id: string;
    name: string;
    role: Role;
}

const initialState = {
    user: null as null | User,
};

export const profileSlice = createSlice({
    name: 'profile',
    initialState,
    reducers: {
        setUser: (state, action: PayloadAction<User>) => {
            state.user = action.payload;
        },
    },
});
