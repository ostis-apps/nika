"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = exports.RadioOptionTypeContextProvider = exports.RadioOptionTypeContext = exports.RadioGroupContextProvider = void 0;

var React = _interopRequireWildcard(require("react"));

var RadioGroupContext = /*#__PURE__*/React.createContext(null);
var RadioGroupContextProvider = RadioGroupContext.Provider;
exports.RadioGroupContextProvider = RadioGroupContextProvider;
var _default = RadioGroupContext;
exports["default"] = _default;
var RadioOptionTypeContext = /*#__PURE__*/React.createContext(null);
exports.RadioOptionTypeContext = RadioOptionTypeContext;
var RadioOptionTypeContextProvider = RadioOptionTypeContext.Provider;
exports.RadioOptionTypeContextProvider = RadioOptionTypeContextProvider;