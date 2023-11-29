"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = useDebounce;

var _slicedToArray2 = _interopRequireDefault(require("@babel/runtime/helpers/slicedToArray"));

var React = _interopRequireWildcard(require("react"));

function useDebounce(value) {
  var _React$useState = React.useState(value),
      _React$useState2 = (0, _slicedToArray2["default"])(_React$useState, 2),
      cacheValue = _React$useState2[0],
      setCacheValue = _React$useState2[1];

  React.useEffect(function () {
    var timeout = setTimeout(function () {
      setCacheValue(value);
    }, value.length ? 0 : 10);
    return function () {
      clearTimeout(timeout);
    };
  }, [value]);
  return cacheValue;
}