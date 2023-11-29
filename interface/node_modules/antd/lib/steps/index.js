"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _CheckOutlined = _interopRequireDefault(require("@ant-design/icons/CheckOutlined"));

var _CloseOutlined = _interopRequireDefault(require("@ant-design/icons/CloseOutlined"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcSteps = _interopRequireDefault(require("rc-steps"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _useBreakpoint2 = _interopRequireDefault(require("../grid/hooks/useBreakpoint"));

var _progress = _interopRequireDefault(require("../progress"));

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

var Steps = function Steps(props) {
  var _classNames;

  var percent = props.percent,
      size = props.size,
      className = props.className,
      direction = props.direction,
      responsive = props.responsive,
      restProps = __rest(props, ["percent", "size", "className", "direction", "responsive"]);

  var _useBreakpoint = (0, _useBreakpoint2["default"])(responsive),
      xs = _useBreakpoint.xs;

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      rtlDirection = _React$useContext.direction;

  var getDirection = React.useCallback(function () {
    return responsive && xs ? 'vertical' : direction;
  }, [xs, direction]);
  var prefixCls = getPrefixCls('steps', props.prefixCls);
  var iconPrefix = getPrefixCls('', props.iconPrefix);
  var stepsClassName = (0, _classnames["default"])((_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-rtl"), rtlDirection === 'rtl'), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-with-progress"), percent !== undefined), _classNames), className);
  var icons = {
    finish: /*#__PURE__*/React.createElement(_CheckOutlined["default"], {
      className: "".concat(prefixCls, "-finish-icon")
    }),
    error: /*#__PURE__*/React.createElement(_CloseOutlined["default"], {
      className: "".concat(prefixCls, "-error-icon")
    })
  };

  var stepIconRender = function stepIconRender(_ref) {
    var node = _ref.node,
        status = _ref.status;

    if (status === 'process' && percent !== undefined) {
      // currently it's hard-coded, since we can't easily read the actually width of icon
      var progressWidth = size === 'small' ? 32 : 40;
      var iconWithProgress = /*#__PURE__*/React.createElement("div", {
        className: "".concat(prefixCls, "-progress-icon")
      }, /*#__PURE__*/React.createElement(_progress["default"], {
        type: "circle",
        percent: percent,
        width: progressWidth,
        strokeWidth: 4,
        format: function format() {
          return null;
        }
      }), node);
      return iconWithProgress;
    }

    return node;
  };

  return /*#__PURE__*/React.createElement(_rcSteps["default"], (0, _extends2["default"])({
    icons: icons
  }, restProps, {
    size: size,
    direction: getDirection(),
    stepIcon: stepIconRender,
    prefixCls: prefixCls,
    iconPrefix: iconPrefix,
    className: stepsClassName
  }));
};

Steps.Step = _rcSteps["default"].Step;
Steps.defaultProps = {
  current: 0,
  responsive: true
};
var _default = Steps;
exports["default"] = _default;