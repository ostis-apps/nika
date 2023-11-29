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

var _ref = require("rc-util/lib/ref");

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _warning = _interopRequireDefault(require("../_util/warning"));

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

var Typography = function Typography(_a, ref) {
  var customizePrefixCls = _a.prefixCls,
      _a$component = _a.component,
      component = _a$component === void 0 ? 'article' : _a$component,
      className = _a.className,
      ariaLabel = _a['aria-label'],
      setContentRef = _a.setContentRef,
      children = _a.children,
      restProps = __rest(_a, ["prefixCls", "component", "className", 'aria-label', "setContentRef", "children"]);

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction;

  var mergedRef = ref;

  if (setContentRef) {
    process.env.NODE_ENV !== "production" ? (0, _warning["default"])(false, 'Typography', '`setContentRef` is deprecated. Please use `ref` instead.') : void 0;
    mergedRef = (0, _ref.composeRef)(ref, setContentRef);
  }

  var Component = component;
  var prefixCls = getPrefixCls('typography', customizePrefixCls);
  var componentClassName = (0, _classnames["default"])(prefixCls, (0, _defineProperty2["default"])({}, "".concat(prefixCls, "-rtl"), direction === 'rtl'), className);
  return /*#__PURE__*/React.createElement(Component, (0, _extends2["default"])({
    className: componentClassName,
    "aria-label": ariaLabel,
    ref: mergedRef
  }, restProps), children);
};

var RefTypography = /*#__PURE__*/React.forwardRef(Typography);

if (process.env.NODE_ENV !== 'production') {
  RefTypography.displayName = 'Typography';
} // es default export should use const instead of let


var ExportTypography = RefTypography;
var _default = ExportTypography;
exports["default"] = _default;