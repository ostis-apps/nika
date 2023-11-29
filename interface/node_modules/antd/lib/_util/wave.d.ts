import * as React from 'react';
import type { ConfigConsumerProps } from '../config-provider';
export interface WaveProps {
    insertExtraNode?: boolean;
    disabled?: boolean;
    children?: React.ReactNode;
}
declare class InternalWave extends React.Component<WaveProps> {
    static contextType: React.Context<ConfigConsumerProps>;
    private instance?;
    private containerRef;
    private extraNode;
    private clickWaveTimeoutId;
    private animationStartId;
    private animationStart;
    private destroyed;
    private csp?;
    context: ConfigConsumerProps;
    componentDidMount(): void;
    componentWillUnmount(): void;
    onClick: (node: HTMLElement, waveColor: string) => void;
    onTransitionStart: (e: AnimationEvent) => void;
    onTransitionEnd: (e: AnimationEvent) => void;
    getAttributeName(): string;
    bindAnimationEvent: (node: HTMLElement) => {
        cancel: () => void;
    } | undefined;
    resetEffect(node: HTMLElement): void;
    renderWave: ({ csp }: ConfigConsumerProps) => string | number | boolean | React.ReactFragment | React.ReactElement<any, string | React.JSXElementConstructor<any>> | null | undefined;
    render(): JSX.Element;
}
declare const Wave: React.ForwardRefExoticComponent<WaveProps & React.RefAttributes<InternalWave>>;
export default Wave;
