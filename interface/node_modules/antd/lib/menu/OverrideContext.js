"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = exports.OverrideProvider = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var React = _interopRequireWildcard(require("react"));

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

/** @private Internal Usage. Only used for Dropdown component. Do not use this in your production. */
var OverrideContext = /*#__PURE__*/React.createContext(null);
/** @private Internal Usage. Only used for Dropdown component. Do not use this in your production. */

var OverrideProvider = function OverrideProvider(_a) {
  var children = _a.children,
      restProps = __rest(_a, ["children"]);

  var override = React.useContext(OverrideContext);
  var context = React.useMemo(function () {
    return (0, _extends2["default"])((0, _extends2["default"])({}, override), restProps);
  }, [override, restProps.prefixCls, // restProps.expandIcon, Not mark as deps since this is a ReactNode
  restProps.mode, restProps.selectable // restProps.validator, Not mark as deps since this is a function
  ]);
  return /*#__PURE__*/React.createElement(OverrideContext.Provider, {
    value: context
  }, children);
};

exports.OverrideProvider = OverrideProvider;
var _default = OverrideContext;
exports["default"] = _default;