import { useCallback, useEffect, useState } from 'react';

interface IParams {
    updateable?: boolean;
}

const defaultParams: IParams = {
    updateable: false,
};

export const useBooleanState = (defaultState: boolean, params: IParams = defaultParams) => {
    const [value, setValue] = useState(defaultState);
    const setFalse = useCallback(() => setValue(false), []);
    const setTrue = useCallback(() => setValue(true), []);
    const toggle = useCallback(() => setValue((prev) => !prev), []);

    useEffect(() => {
        if (params.updateable) setValue(defaultState);
    }, [defaultState, params.updateable]);

    return [value, setTrue, setFalse, toggle] as const;
};
