"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = generateRangePicker;

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _CalendarOutlined = _interopRequireDefault(require("@ant-design/icons/CalendarOutlined"));

var _ClockCircleOutlined = _interopRequireDefault(require("@ant-design/icons/ClockCircleOutlined"));

var _CloseCircleFilled = _interopRequireDefault(require("@ant-design/icons/CloseCircleFilled"));

var _SwapRightOutlined = _interopRequireDefault(require("@ant-design/icons/SwapRightOutlined"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcPicker = require("rc-picker");

var React = _interopRequireWildcard(require("react"));

var _ = require(".");

var _configProvider = require("../../config-provider");

var _DisabledContext = _interopRequireDefault(require("../../config-provider/DisabledContext"));

var _SizeContext = _interopRequireDefault(require("../../config-provider/SizeContext"));

var _context = require("../../form/context");

var _LocaleReceiver = _interopRequireDefault(require("../../locale-provider/LocaleReceiver"));

var _statusUtils = require("../../_util/statusUtils");

var _en_US = _interopRequireDefault(require("../locale/en_US"));

var _util = require("../util");

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

function generateRangePicker(generateConfig) {
  var RangePicker = /*#__PURE__*/(0, React.forwardRef)(function (props, ref) {
    var customizePrefixCls = props.prefixCls,
        customGetPopupContainer = props.getPopupContainer,
        className = props.className,
        placement = props.placement,
        customizeSize = props.size,
        customDisabled = props.disabled,
        _props$bordered = props.bordered,
        bordered = _props$bordered === void 0 ? true : _props$bordered,
        placeholder = props.placeholder,
        customStatus = props.status,
        restProps = __rest(props, ["prefixCls", "getPopupContainer", "className", "placement", "size", "disabled", "bordered", "placeholder", "status"]);

    var innerRef = React.useRef(null);

    var _useContext = (0, React.useContext)(_configProvider.ConfigContext),
        getPrefixCls = _useContext.getPrefixCls,
        direction = _useContext.direction,
        getPopupContainer = _useContext.getPopupContainer;

    var prefixCls = getPrefixCls('picker', customizePrefixCls);
    var format = props.format,
        showTime = props.showTime,
        picker = props.picker;
    var rootPrefixCls = getPrefixCls();
    var additionalOverrideProps = {};
    additionalOverrideProps = (0, _extends2["default"])((0, _extends2["default"])((0, _extends2["default"])({}, additionalOverrideProps), showTime ? (0, _.getTimeProps)((0, _extends2["default"])({
      format: format,
      picker: picker
    }, showTime)) : {}), picker === 'time' ? (0, _.getTimeProps)((0, _extends2["default"])((0, _extends2["default"])({
      format: format
    }, props), {
      picker: picker
    })) : {}); // ===================== Size =====================

    var size = React.useContext(_SizeContext["default"]);
    var mergedSize = customizeSize || size; // ===================== Disabled =====================

    var disabled = React.useContext(_DisabledContext["default"]);
    var mergedDisabled = customDisabled || disabled; // ===================== FormItemInput =====================

    var formItemContext = (0, React.useContext)(_context.FormItemInputContext);
    var hasFeedback = formItemContext.hasFeedback,
        contextStatus = formItemContext.status,
        feedbackIcon = formItemContext.feedbackIcon;
    var suffixNode = /*#__PURE__*/React.createElement(React.Fragment, null, picker === 'time' ? /*#__PURE__*/React.createElement(_ClockCircleOutlined["default"], null) : /*#__PURE__*/React.createElement(_CalendarOutlined["default"], null), hasFeedback && feedbackIcon);
    (0, React.useImperativeHandle)(ref, function () {
      return {
        focus: function focus() {
          var _a;

          return (_a = innerRef.current) === null || _a === void 0 ? void 0 : _a.focus();
        },
        blur: function blur() {
          var _a;

          return (_a = innerRef.current) === null || _a === void 0 ? void 0 : _a.blur();
        }
      };
    });
    return /*#__PURE__*/React.createElement(_LocaleReceiver["default"], {
      componentName: "DatePicker",
      defaultLocale: _en_US["default"]
    }, function (contextLocale) {
      var _classNames;

      var locale = (0, _extends2["default"])((0, _extends2["default"])({}, contextLocale), props.locale);
      return /*#__PURE__*/React.createElement(_rcPicker.RangePicker, (0, _extends2["default"])({
        separator: /*#__PURE__*/React.createElement("span", {
          "aria-label": "to",
          className: "".concat(prefixCls, "-separator")
        }, /*#__PURE__*/React.createElement(_SwapRightOutlined["default"], null)),
        disabled: mergedDisabled,
        ref: innerRef,
        dropdownAlign: (0, _util.transPlacement2DropdownAlign)(direction, placement),
        placeholder: (0, _util.getRangePlaceholder)(picker, locale, placeholder),
        suffixIcon: suffixNode,
        clearIcon: /*#__PURE__*/React.createElement(_CloseCircleFilled["default"], null),
        prevIcon: /*#__PURE__*/React.createElement("span", {
          className: "".concat(prefixCls, "-prev-icon")
        }),
        nextIcon: /*#__PURE__*/React.createElement("span", {
          className: "".concat(prefixCls, "-next-icon")
        }),
        superPrevIcon: /*#__PURE__*/React.createElement("span", {
          className: "".concat(prefixCls, "-super-prev-icon")
        }),
        superNextIcon: /*#__PURE__*/React.createElement("span", {
          className: "".concat(prefixCls, "-super-next-icon")
        }),
        allowClear: true,
        transitionName: "".concat(rootPrefixCls, "-slide-up")
      }, restProps, additionalOverrideProps, {
        className: (0, _classnames["default"])((_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-").concat(mergedSize), mergedSize), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-borderless"), !bordered), _classNames), (0, _statusUtils.getStatusClassNames)(prefixCls, (0, _statusUtils.getMergedStatus)(contextStatus, customStatus), hasFeedback), className),
        locale: locale.lang,
        prefixCls: prefixCls,
        getPopupContainer: customGetPopupContainer || getPopupContainer,
        generateConfig: generateConfig,
        components: _.Components,
        direction: direction
      }));
    });
  });
  return RangePicker;
}