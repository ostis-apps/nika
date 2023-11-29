import * as React from 'react';
import type Countdown from './Countdown';
import type { FormatConfig, valueType } from './utils';
interface StatisticComponent {
    Countdown: typeof Countdown;
}
export interface StatisticProps extends FormatConfig {
    prefixCls?: string;
    className?: string;
    style?: React.CSSProperties;
    value?: valueType;
    valueStyle?: React.CSSProperties;
    valueRender?: (node: React.ReactNode) => React.ReactNode;
    title?: React.ReactNode;
    prefix?: React.ReactNode;
    suffix?: React.ReactNode;
    loading?: boolean;
    onMouseEnter?: React.MouseEventHandler<HTMLDivElement>;
    onMouseLeave?: React.MouseEventHandler<HTMLDivElement>;
}
declare const WrapperStatistic: React.FC<StatisticProps> & StatisticComponent;
export default WrapperStatistic;
