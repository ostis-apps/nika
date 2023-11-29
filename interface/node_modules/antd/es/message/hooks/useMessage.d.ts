import type { NoticeContent as RCNoticeContent, NotificationInstance as RCNotificationInstance } from 'rc-notification/lib/Notification';
import * as React from 'react';
import type { ArgsProps, MessageInstance } from '..';
export default function createUseMessage(getRcNotificationInstance: (args: ArgsProps, callback: (info: {
    prefixCls: string;
    instance: RCNotificationInstance;
}) => void) => void, getRCNoticeProps: (args: ArgsProps, prefixCls: string) => RCNoticeContent): () => [MessageInstance, React.ReactElement];
