"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.MiniSelect = exports.MiddleSelect = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var React = _interopRequireWildcard(require("react"));

var _select = _interopRequireDefault(require("../select"));

var MiniSelect = function MiniSelect(props) {
  return /*#__PURE__*/React.createElement(_select["default"], (0, _extends2["default"])({}, props, {
    size: "small"
  }));
};

exports.MiniSelect = MiniSelect;

var MiddleSelect = function MiddleSelect(props) {
  return /*#__PURE__*/React.createElement(_select["default"], (0, _extends2["default"])({}, props, {
    size: "middle"
  }));
};

exports.MiddleSelect = MiddleSelect;
MiniSelect.Option = _select["default"].Option;
MiddleSelect.Option = _select["default"].Option;