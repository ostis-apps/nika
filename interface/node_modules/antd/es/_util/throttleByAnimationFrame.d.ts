export declare function throttleByAnimationFrame<T extends unknown[]>(fn: (...args: T) => void): {
    (...args: T): void;
    cancel: () => void;
};
export declare function throttleByAnimationFrameDecorator(): (target: any, key: string, descriptor: any) => {
    configurable: boolean;
    get(): any;
};
