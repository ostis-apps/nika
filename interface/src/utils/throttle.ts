export const throttle = <F extends (...args: any[]) => any>(func: F, delay: number) => {
    let isRuning = false;
    let isFirstTime = true;
    return (...args: Parameters<F>) => {
        if (isFirstTime || !isRuning) {
            func(...args);
            setTimeout(() => {
                isRuning = false;
            }, delay);
        }
        isRuning = true;
        isFirstTime = false;
    };
};
