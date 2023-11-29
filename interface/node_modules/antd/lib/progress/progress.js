"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _CheckCircleFilled = _interopRequireDefault(require("@ant-design/icons/CheckCircleFilled"));

var _CheckOutlined = _interopRequireDefault(require("@ant-design/icons/CheckOutlined"));

var _CloseCircleFilled = _interopRequireDefault(require("@ant-design/icons/CloseCircleFilled"));

var _CloseOutlined = _interopRequireDefault(require("@ant-design/icons/CloseOutlined"));

var _classnames = _interopRequireDefault(require("classnames"));

var _omit = _interopRequireDefault(require("rc-util/lib/omit"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _type = require("../_util/type");

var _warning = _interopRequireDefault(require("../_util/warning"));

var _Circle = _interopRequireDefault(require("./Circle"));

var _Line = _interopRequireDefault(require("./Line"));

var _Steps = _interopRequireDefault(require("./Steps"));

var _utils = require("./utils");

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

var ProgressTypes = (0, _type.tuple)('line', 'circle', 'dashboard');
var ProgressStatuses = (0, _type.tuple)('normal', 'exception', 'active', 'success');

var Progress = function Progress(props) {
  var _classNames;

  var customizePrefixCls = props.prefixCls,
      className = props.className,
      steps = props.steps,
      strokeColor = props.strokeColor,
      _props$percent = props.percent,
      percent = _props$percent === void 0 ? 0 : _props$percent,
      _props$size = props.size,
      size = _props$size === void 0 ? 'default' : _props$size,
      _props$showInfo = props.showInfo,
      showInfo = _props$showInfo === void 0 ? true : _props$showInfo,
      _props$type = props.type,
      type = _props$type === void 0 ? 'line' : _props$type,
      restProps = __rest(props, ["prefixCls", "className", "steps", "strokeColor", "percent", "size", "showInfo", "type"]);

  function getPercentNumber() {
    var successPercent = (0, _utils.getSuccessPercent)(props);
    return parseInt(successPercent !== undefined ? successPercent.toString() : percent.toString(), 10);
  }

  function getProgressStatus() {
    var status = props.status;

    if (ProgressStatuses.indexOf(status) < 0 && getPercentNumber() >= 100) {
      return 'success';
    }

    return status || 'normal';
  }

  function renderProcessInfo(prefixCls, progressStatus) {
    var format = props.format;
    var successPercent = (0, _utils.getSuccessPercent)(props);

    if (!showInfo) {
      return null;
    }

    var text;

    var textFormatter = format || function (percentNumber) {
      return "".concat(percentNumber, "%");
    };

    var isLineType = type === 'line';

    if (format || progressStatus !== 'exception' && progressStatus !== 'success') {
      text = textFormatter((0, _utils.validProgress)(percent), (0, _utils.validProgress)(successPercent));
    } else if (progressStatus === 'exception') {
      text = isLineType ? /*#__PURE__*/React.createElement(_CloseCircleFilled["default"], null) : /*#__PURE__*/React.createElement(_CloseOutlined["default"], null);
    } else if (progressStatus === 'success') {
      text = isLineType ? /*#__PURE__*/React.createElement(_CheckCircleFilled["default"], null) : /*#__PURE__*/React.createElement(_CheckOutlined["default"], null);
    }

    return /*#__PURE__*/React.createElement("span", {
      className: "".concat(prefixCls, "-text"),
      title: typeof text === 'string' ? text : undefined
    }, text);
  }

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction;

  var prefixCls = getPrefixCls('progress', customizePrefixCls);
  var progressStatus = getProgressStatus();
  var progressInfo = renderProcessInfo(prefixCls, progressStatus);
  process.env.NODE_ENV !== "production" ? (0, _warning["default"])(!('successPercent' in props), 'Progress', '`successPercent` is deprecated. Please use `success.percent` instead.') : void 0;
  var strokeColorNotArray = Array.isArray(strokeColor) ? strokeColor[0] : strokeColor;
  var strokeColorNotGradient = typeof strokeColor === 'string' || Array.isArray(strokeColor) ? strokeColor : undefined;
  var progress; // Render progress shape

  if (type === 'line') {
    progress = steps ? /*#__PURE__*/React.createElement(_Steps["default"], (0, _extends2["default"])({}, props, {
      strokeColor: strokeColorNotGradient,
      prefixCls: prefixCls,
      steps: steps
    }), progressInfo) : /*#__PURE__*/React.createElement(_Line["default"], (0, _extends2["default"])({}, props, {
      strokeColor: strokeColorNotArray,
      prefixCls: prefixCls,
      direction: direction
    }), progressInfo);
  } else if (type === 'circle' || type === 'dashboard') {
    progress = /*#__PURE__*/React.createElement(_Circle["default"], (0, _extends2["default"])({}, props, {
      strokeColor: strokeColorNotArray,
      prefixCls: prefixCls,
      progressStatus: progressStatus
    }), progressInfo);
  }

  var classString = (0, _classnames["default"])(prefixCls, (_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-").concat(type === 'dashboard' && 'circle' || steps && 'steps' || type), true), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-status-").concat(progressStatus), true), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-show-info"), showInfo), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-").concat(size), size), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-rtl"), direction === 'rtl'), _classNames), className);
  return /*#__PURE__*/React.createElement("div", (0, _extends2["default"])({}, (0, _omit["default"])(restProps, ['status', 'format', 'trailColor', 'strokeWidth', 'width', 'gapDegree', 'gapPosition', 'strokeLinecap', 'success', 'successPercent']), {
    className: classString
  }), progress);
};

var _default = Progress;
exports["default"] = _default;