"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _typeof2 = _interopRequireDefault(require("@babel/runtime/helpers/typeof"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcSegmented = _interopRequireDefault(require("rc-segmented"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _SizeContext = _interopRequireDefault(require("../config-provider/SizeContext"));

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

function isSegmentedLabeledOptionWithIcon(option) {
  return (0, _typeof2["default"])(option) === 'object' && !!(option === null || option === void 0 ? void 0 : option.icon);
}

var Segmented = /*#__PURE__*/React.forwardRef(function (props, ref) {
  var _classNames;

  var customizePrefixCls = props.prefixCls,
      className = props.className,
      block = props.block,
      options = props.options,
      _props$size = props.size,
      customSize = _props$size === void 0 ? 'middle' : _props$size,
      restProps = __rest(props, ["prefixCls", "className", "block", "options", "size"]);

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction;

  var prefixCls = getPrefixCls('segmented', customizePrefixCls); // ===================== Size =====================

  var size = React.useContext(_SizeContext["default"]);
  var mergedSize = customSize || size; // syntactic sugar to support `icon` for Segmented Item

  var extendedOptions = React.useMemo(function () {
    return options.map(function (option) {
      if (isSegmentedLabeledOptionWithIcon(option)) {
        var icon = option.icon,
            label = option.label,
            restOption = __rest(option, ["icon", "label"]);

        return (0, _extends2["default"])((0, _extends2["default"])({}, restOption), {
          label: /*#__PURE__*/React.createElement(React.Fragment, null, /*#__PURE__*/React.createElement("span", {
            className: "".concat(prefixCls, "-item-icon")
          }, icon), label && /*#__PURE__*/React.createElement("span", null, label))
        });
      }

      return option;
    });
  }, [options, prefixCls]);
  return /*#__PURE__*/React.createElement(_rcSegmented["default"], (0, _extends2["default"])({}, restProps, {
    className: (0, _classnames["default"])(className, (_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-block"), block), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-sm"), mergedSize === 'small'), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-lg"), mergedSize === 'large'), _classNames)),
    options: extendedOptions,
    ref: ref,
    prefixCls: prefixCls,
    direction: direction
  }));
});

if (process.env.NODE_ENV !== 'production') {
  Segmented.displayName = 'Segmented';
}

Segmented.defaultProps = {
  options: []
};
var _default = Segmented;
exports["default"] = _default;