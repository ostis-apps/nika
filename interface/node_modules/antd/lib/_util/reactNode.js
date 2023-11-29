"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.cloneElement = cloneElement;
exports.isValidElement = void 0;
exports.replaceElement = replaceElement;

var React = _interopRequireWildcard(require("react"));

var isValidElement = React.isValidElement;
exports.isValidElement = isValidElement;

function replaceElement(element, replacement, props) {
  if (!isValidElement(element)) return replacement;
  return /*#__PURE__*/React.cloneElement(element, typeof props === 'function' ? props(element.props || {}) : props);
}

function cloneElement(element, props) {
  return replaceElement(element, element, props);
}