"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _classnames = _interopRequireDefault(require("classnames"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

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

var Divider = function Divider(props) {
  var _classNames;

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction;

  var customizePrefixCls = props.prefixCls,
      _props$type = props.type,
      type = _props$type === void 0 ? 'horizontal' : _props$type,
      _props$orientation = props.orientation,
      orientation = _props$orientation === void 0 ? 'center' : _props$orientation,
      orientationMargin = props.orientationMargin,
      className = props.className,
      children = props.children,
      dashed = props.dashed,
      plain = props.plain,
      restProps = __rest(props, ["prefixCls", "type", "orientation", "orientationMargin", "className", "children", "dashed", "plain"]);

  var prefixCls = getPrefixCls('divider', customizePrefixCls);
  var orientationPrefix = orientation.length > 0 ? "-".concat(orientation) : orientation;
  var hasChildren = !!children;
  var hasCustomMarginLeft = orientation === 'left' && orientationMargin != null;
  var hasCustomMarginRight = orientation === 'right' && orientationMargin != null;
  var classString = (0, _classnames["default"])(prefixCls, "".concat(prefixCls, "-").concat(type), (_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-with-text"), hasChildren), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-with-text").concat(orientationPrefix), hasChildren), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-dashed"), !!dashed), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-plain"), !!plain), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-rtl"), direction === 'rtl'), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-no-default-orientation-margin-left"), hasCustomMarginLeft), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-no-default-orientation-margin-right"), hasCustomMarginRight), _classNames), className);
  var innerStyle = (0, _extends2["default"])((0, _extends2["default"])({}, hasCustomMarginLeft && {
    marginLeft: orientationMargin
  }), hasCustomMarginRight && {
    marginRight: orientationMargin
  });
  return /*#__PURE__*/React.createElement("div", (0, _extends2["default"])({
    className: classString
  }, restProps, {
    role: "separator"
  }), children && /*#__PURE__*/React.createElement("span", {
    className: "".concat(prefixCls, "-inner-text"),
    style: innerStyle
  }, children));
};

var _default = Divider;
exports["default"] = _default;