"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _DotChartOutlined = _interopRequireDefault(require("@ant-design/icons/DotChartOutlined"));

var _classnames = _interopRequireDefault(require("classnames"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var SkeletonNode = function SkeletonNode(props) {
  var _a;

  var customizePrefixCls = props.prefixCls,
      className = props.className,
      style = props.style,
      active = props.active;

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls;

  var prefixCls = getPrefixCls('skeleton', customizePrefixCls);
  var cls = (0, _classnames["default"])(prefixCls, "".concat(prefixCls, "-element"), (0, _defineProperty2["default"])({}, "".concat(prefixCls, "-active"), active), className);
  var content = (_a = props.children) !== null && _a !== void 0 ? _a : /*#__PURE__*/React.createElement(_DotChartOutlined["default"], null);
  return /*#__PURE__*/React.createElement("div", {
    className: cls
  }, /*#__PURE__*/React.createElement("div", {
    className: (0, _classnames["default"])("".concat(prefixCls, "-image"), className),
    style: style
  }, content));
};

var _default = SkeletonNode;
exports["default"] = _default;