"use strict";

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _slicedToArray2 = _interopRequireDefault(require("@babel/runtime/helpers/slicedToArray"));

var _ExclamationCircleFilled = _interopRequireDefault(require("@ant-design/icons/ExclamationCircleFilled"));

var _classnames = _interopRequireDefault(require("classnames"));

var _useMergedState3 = _interopRequireDefault(require("rc-util/lib/hooks/useMergedState"));

var _KeyCode = _interopRequireDefault(require("rc-util/lib/KeyCode"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _popover = _interopRequireDefault(require("../popover"));

var _reactNode = require("../_util/reactNode");

var _PurePanel = require("./PurePanel");

var _this = void 0;

var __rest = void 0 && (void 0).__rest || function (s, e) {
  var t = {};

  for (var p in s) {
    if (Object.prototype.hasOwnProperty.call(s, p) && e.indexOf(p) < 0) t[p] = s[p];
  }

  if (s != null && typeof Object.getOwnPropertySymbols === "function") for (var i = 0, p = Object.getOwnPropertySymbols(s); i < p.length; i++) {
    if (e.indexOf(p[i]) < 0 && Object.prototype.propertyIsEnumerable.call(s, p[i])) t[p[i]] = s[p[i]];
  }
  return t;
};

var Popconfirm = /*#__PURE__*/React.forwardRef(function (props, ref) {
  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls;

  var _useMergedState = (0, _useMergedState3["default"])(false, {
    value: props.visible,
    defaultValue: props.defaultVisible
  }),
      _useMergedState2 = (0, _slicedToArray2["default"])(_useMergedState, 2),
      visible = _useMergedState2[0],
      setVisible = _useMergedState2[1]; // const isDestroyed = useDestroyed();


  var settingVisible = function settingVisible(value, e) {
    var _a;

    setVisible(value, true);
    (_a = props.onVisibleChange) === null || _a === void 0 ? void 0 : _a.call(props, value, e);
  };

  var close = function close(e) {
    settingVisible(false, e);
  };

  var onConfirm = function onConfirm(e) {
    var _a;

    return (_a = props.onConfirm) === null || _a === void 0 ? void 0 : _a.call(_this, e);
  };

  var onCancel = function onCancel(e) {
    var _a;

    settingVisible(false, e);
    (_a = props.onCancel) === null || _a === void 0 ? void 0 : _a.call(_this, e);
  };

  var _onKeyDown = function onKeyDown(e) {
    if (e.keyCode === _KeyCode["default"].ESC && visible) {
      settingVisible(false, e);
    }
  };

  var onVisibleChange = function onVisibleChange(value) {
    var disabled = props.disabled;

    if (disabled) {
      return;
    }

    settingVisible(value);
  };

  var customizePrefixCls = props.prefixCls,
      placement = props.placement,
      children = props.children,
      overlayClassName = props.overlayClassName,
      restProps = __rest(props, ["prefixCls", "placement", "children", "overlayClassName"]);

  var prefixCls = getPrefixCls('popover', customizePrefixCls);
  var prefixClsConfirm = getPrefixCls('popconfirm', customizePrefixCls);
  var overlayClassNames = (0, _classnames["default"])(prefixClsConfirm, overlayClassName);
  return /*#__PURE__*/React.createElement(_popover["default"], (0, _extends2["default"])({}, restProps, {
    prefixCls: prefixCls,
    placement: placement,
    onVisibleChange: onVisibleChange,
    visible: visible,
    _overlay: /*#__PURE__*/React.createElement(_PurePanel.Overlay, (0, _extends2["default"])({}, props, {
      prefixCls: prefixCls,
      close: close,
      onConfirm: onConfirm,
      onCancel: onCancel
    })),
    overlayClassName: overlayClassNames,
    ref: ref
  }), (0, _reactNode.cloneElement)(children, {
    onKeyDown: function onKeyDown(e) {
      var _a, _b;

      if ( /*#__PURE__*/React.isValidElement(children)) {
        (_b = children === null || children === void 0 ? void 0 : (_a = children.props).onKeyDown) === null || _b === void 0 ? void 0 : _b.call(_a, e);
      }

      _onKeyDown(e);
    }
  }));
});
Popconfirm.defaultProps = {
  placement: 'top',
  trigger: 'click',
  okType: 'primary',
  icon: /*#__PURE__*/React.createElement(_ExclamationCircleFilled["default"], null),
  disabled: false
};
var _default = Popconfirm;
exports["default"] = _default;