import * as React from 'react';
import type { PopconfirmProps } from '.';
export interface PopconfirmLocale {
    okText: string;
    cancelText: string;
}
export interface OverlayProps extends Pick<PopconfirmProps, 'icon' | 'okButtonProps' | 'cancelButtonProps' | 'cancelText' | 'okText' | 'okType' | 'showCancel' | 'title'> {
    prefixCls: string;
    close?: Function;
    onConfirm?: React.MouseEventHandler<HTMLButtonElement>;
    onCancel?: React.MouseEventHandler<HTMLButtonElement>;
}
export declare function Overlay(props: OverlayProps): JSX.Element;
