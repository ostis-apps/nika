"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var React = _interopRequireWildcard(require("react"));

var _Base = _interopRequireDefault(require("./Base"));

var Paragraph = function Paragraph(props, ref) {
  return /*#__PURE__*/React.createElement(_Base["default"], (0, _extends2["default"])({
    ref: ref
  }, props, {
    component: "div"
  }));
};

var _default = /*#__PURE__*/React.forwardRef(Paragraph);

exports["default"] = _default;