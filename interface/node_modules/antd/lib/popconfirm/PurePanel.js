"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.Overlay = Overlay;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var React = _interopRequireWildcard(require("react"));

var _button = _interopRequireDefault(require("../button"));

var _button2 = require("../button/button");

var _ActionButton = _interopRequireDefault(require("../_util/ActionButton"));

var _LocaleReceiver = _interopRequireDefault(require("../locale-provider/LocaleReceiver"));

var _default = _interopRequireDefault(require("../locale/default"));

var _getRenderPropValue = require("../_util/getRenderPropValue");

var _configProvider = require("../config-provider");

function Overlay(props) {
  var prefixCls = props.prefixCls,
      okButtonProps = props.okButtonProps,
      cancelButtonProps = props.cancelButtonProps,
      title = props.title,
      cancelText = props.cancelText,
      okText = props.okText,
      okType = props.okType,
      icon = props.icon,
      _props$showCancel = props.showCancel,
      showCancel = _props$showCancel === void 0 ? true : _props$showCancel,
      close = props.close,
      onConfirm = props.onConfirm,
      onCancel = props.onCancel;

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls;

  return /*#__PURE__*/React.createElement(_LocaleReceiver["default"], {
    componentName: "Popconfirm",
    defaultLocale: _default["default"].Popconfirm
  }, function (popconfirmLocale) {
    return /*#__PURE__*/React.createElement("div", {
      className: "".concat(prefixCls, "-inner-content")
    }, /*#__PURE__*/React.createElement("div", {
      className: "".concat(prefixCls, "-message")
    }, icon, /*#__PURE__*/React.createElement("div", {
      className: "".concat(prefixCls, "-message-title")
    }, (0, _getRenderPropValue.getRenderPropValue)(title))), /*#__PURE__*/React.createElement("div", {
      className: "".concat(prefixCls, "-buttons")
    }, showCancel && /*#__PURE__*/React.createElement(_button["default"], (0, _extends2["default"])({
      onClick: onCancel,
      size: "small"
    }, cancelButtonProps), cancelText || popconfirmLocale.cancelText), /*#__PURE__*/React.createElement(_ActionButton["default"], {
      buttonProps: (0, _extends2["default"])((0, _extends2["default"])({
        size: 'small'
      }, (0, _button2.convertLegacyProps)(okType)), okButtonProps),
      actionFn: onConfirm,
      close: close,
      prefixCls: getPrefixCls('btn'),
      quitOnNullishReturnValue: true,
      emitEvent: true
    }, okText || popconfirmLocale.okText)));
  });
}