import { RefObject, useCallback, useEffect } from 'react';

export const useClickOutside = (
    elementOrElements: RefObject<HTMLElement> | RefObject<HTMLElement>[],
    handler: (e: Event) => void,
) => {
    const listener = useCallback(
        (e: MouseEvent | TouchEvent) => {
            const elements = Array.isArray(elementOrElements) ? elementOrElements : [elementOrElements];
            const isOutside = elements.every((element) => !element.current?.contains(e.target as Element));

            if (isOutside) {
                handler(e);
            }
        },
        [handler, elementOrElements],
    );

    useEffect(() => {
        document.addEventListener('mousedown', listener, true);
        document.addEventListener('touchstart', listener);

        return () => {
            document.removeEventListener('mousedown', listener, true);
            document.removeEventListener('touchstart', listener);
        };
    }, [listener]);
};
