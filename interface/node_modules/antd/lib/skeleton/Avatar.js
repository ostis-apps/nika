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

var _omit = _interopRequireDefault(require("rc-util/lib/omit"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _Element = _interopRequireDefault(require("./Element"));

var SkeletonAvatar = function SkeletonAvatar(props) {
  var customizePrefixCls = props.prefixCls,
      className = props.className,
      active = props.active;

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls;

  var prefixCls = getPrefixCls('skeleton', customizePrefixCls);
  var otherProps = (0, _omit["default"])(props, ['prefixCls', 'className']);
  var cls = (0, _classnames["default"])(prefixCls, "".concat(prefixCls, "-element"), (0, _defineProperty2["default"])({}, "".concat(prefixCls, "-active"), active), className);
  return /*#__PURE__*/React.createElement("div", {
    className: cls
  }, /*#__PURE__*/React.createElement(_Element["default"], (0, _extends2["default"])({
    prefixCls: "".concat(prefixCls, "-avatar")
  }, otherProps)));
};

SkeletonAvatar.defaultProps = {
  size: 'default',
  shape: 'circle'
};
var _default = SkeletonAvatar;
exports["default"] = _default;