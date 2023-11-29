"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _classCallCheck2 = _interopRequireDefault(require("@babel/runtime/helpers/classCallCheck"));

var _createClass2 = _interopRequireDefault(require("@babel/runtime/helpers/createClass"));

var _inherits2 = _interopRequireDefault(require("@babel/runtime/helpers/inherits"));

var _createSuper2 = _interopRequireDefault(require("@babel/runtime/helpers/createSuper"));

var _CloseCircleFilled = _interopRequireDefault(require("@ant-design/icons/CloseCircleFilled"));

var _classnames = _interopRequireDefault(require("classnames"));

var React = _interopRequireWildcard(require("react"));

var _context = require("../form/context");

var _reactNode = require("../_util/reactNode");

var _statusUtils = require("../_util/statusUtils");

var _type = require("../_util/type");

var ClearableInputType = (0, _type.tuple)('text', 'input');

function hasAddon(props) {
  return !!(props.addonBefore || props.addonAfter);
}

var ClearableLabeledInput = /*#__PURE__*/function (_React$Component) {
  (0, _inherits2["default"])(ClearableLabeledInput, _React$Component);

  var _super = (0, _createSuper2["default"])(ClearableLabeledInput);

  function ClearableLabeledInput() {
    (0, _classCallCheck2["default"])(this, ClearableLabeledInput);
    return _super.apply(this, arguments);
  }

  (0, _createClass2["default"])(ClearableLabeledInput, [{
    key: "renderClearIcon",
    value: function renderClearIcon(prefixCls) {
      var _classNames;

      var _this$props = this.props,
          value = _this$props.value,
          disabled = _this$props.disabled,
          readOnly = _this$props.readOnly,
          handleReset = _this$props.handleReset,
          suffix = _this$props.suffix;
      var needClear = !disabled && !readOnly && value;
      var className = "".concat(prefixCls, "-clear-icon");
      return /*#__PURE__*/React.createElement(_CloseCircleFilled["default"], {
        onClick: handleReset,
        // Do not trigger onBlur when clear input
        // https://github.com/ant-design/ant-design/issues/31200
        onMouseDown: function onMouseDown(e) {
          return e.preventDefault();
        },
        className: (0, _classnames["default"])((_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(className, "-hidden"), !needClear), (0, _defineProperty2["default"])(_classNames, "".concat(className, "-has-suffix"), !!suffix), _classNames), className),
        role: "button"
      });
    }
  }, {
    key: "renderTextAreaWithClearIcon",
    value: function renderTextAreaWithClearIcon(prefixCls, element, statusContext) {
      var _classNames2;

      var _this$props2 = this.props,
          value = _this$props2.value,
          allowClear = _this$props2.allowClear,
          className = _this$props2.className,
          style = _this$props2.style,
          direction = _this$props2.direction,
          bordered = _this$props2.bordered,
          hidden = _this$props2.hidden,
          customStatus = _this$props2.status;
      var contextStatus = statusContext.status,
          hasFeedback = statusContext.hasFeedback;

      if (!allowClear) {
        return (0, _reactNode.cloneElement)(element, {
          value: value
        });
      }

      var affixWrapperCls = (0, _classnames["default"])("".concat(prefixCls, "-affix-wrapper"), "".concat(prefixCls, "-affix-wrapper-textarea-with-clear-btn"), (0, _statusUtils.getStatusClassNames)("".concat(prefixCls, "-affix-wrapper"), (0, _statusUtils.getMergedStatus)(contextStatus, customStatus), hasFeedback), (_classNames2 = {}, (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-affix-wrapper-rtl"), direction === 'rtl'), (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-affix-wrapper-borderless"), !bordered), (0, _defineProperty2["default"])(_classNames2, "".concat(className), !hasAddon(this.props) && className), _classNames2));
      return /*#__PURE__*/React.createElement("span", {
        className: affixWrapperCls,
        style: style,
        hidden: hidden
      }, (0, _reactNode.cloneElement)(element, {
        style: null,
        value: value
      }), this.renderClearIcon(prefixCls));
    }
  }, {
    key: "render",
    value: function render() {
      var _this = this;

      return /*#__PURE__*/React.createElement(_context.FormItemInputContext.Consumer, null, function (statusContext) {
        var _this$props3 = _this.props,
            prefixCls = _this$props3.prefixCls,
            inputType = _this$props3.inputType,
            element = _this$props3.element;

        if (inputType === ClearableInputType[0]) {
          return _this.renderTextAreaWithClearIcon(prefixCls, element, statusContext);
        }
      });
    }
  }]);
  return ClearableLabeledInput;
}(React.Component);

var _default = ClearableLabeledInput;
exports["default"] = _default;