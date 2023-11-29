"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = useMergedConfig;

var _typeof2 = _interopRequireDefault(require("@babel/runtime/helpers/typeof"));

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var React = _interopRequireWildcard(require("react"));

function useMergedConfig(propConfig, templateConfig) {
  return React.useMemo(function () {
    var support = !!propConfig;
    return [support, (0, _extends2["default"])((0, _extends2["default"])({}, templateConfig), support && (0, _typeof2["default"])(propConfig) === 'object' ? propConfig : null)];
  }, [propConfig]);
}