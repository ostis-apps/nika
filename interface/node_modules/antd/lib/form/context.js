"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.NoStyleItemContext = exports.NoFormStyle = exports.FormProvider = exports.FormItemPrefixContext = exports.FormItemInputContext = exports.FormContext = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _rcFieldForm = require("rc-field-form");

var _omit = _interopRequireDefault(require("rc-util/lib/omit"));

var React = _interopRequireWildcard(require("react"));

var FormContext = /*#__PURE__*/React.createContext({
  labelAlign: 'right',
  vertical: false,
  itemRef: function itemRef() {}
});
exports.FormContext = FormContext;
var NoStyleItemContext = /*#__PURE__*/React.createContext(null);
exports.NoStyleItemContext = NoStyleItemContext;

var FormProvider = function FormProvider(props) {
  var providerProps = (0, _omit["default"])(props, ['prefixCls']);
  return /*#__PURE__*/React.createElement(_rcFieldForm.FormProvider, (0, _extends2["default"])({}, providerProps));
};

exports.FormProvider = FormProvider;
var FormItemPrefixContext = /*#__PURE__*/React.createContext({
  prefixCls: ''
});
exports.FormItemPrefixContext = FormItemPrefixContext;
var FormItemInputContext = /*#__PURE__*/React.createContext({});
exports.FormItemInputContext = FormItemInputContext;

var NoFormStyle = function NoFormStyle(_ref) {
  var children = _ref.children,
      status = _ref.status,
      override = _ref.override;
  var formItemInputContext = (0, React.useContext)(FormItemInputContext);
  var newFormItemInputContext = (0, React.useMemo)(function () {
    var newContext = (0, _extends2["default"])({}, formItemInputContext);

    if (override) {
      delete newContext.isFormItemInput;
    }

    if (status) {
      delete newContext.status;
      delete newContext.hasFeedback;
      delete newContext.feedbackIcon;
    }

    return newContext;
  }, [status, override, formItemInputContext]);
  return /*#__PURE__*/React.createElement(FormItemInputContext.Provider, {
    value: newFormItemInputContext
  }, children);
};

exports.NoFormStyle = NoFormStyle;