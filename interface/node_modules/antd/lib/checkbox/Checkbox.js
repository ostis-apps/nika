"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcCheckbox = _interopRequireDefault(require("rc-checkbox"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _context = require("../form/context");

var _warning = _interopRequireDefault(require("../_util/warning"));

var _Group = require("./Group");

var _DisabledContext = _interopRequireDefault(require("../config-provider/DisabledContext"));

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

var InternalCheckbox = function InternalCheckbox(_a, ref) {
  var _classNames;

  var customizePrefixCls = _a.prefixCls,
      className = _a.className,
      children = _a.children,
      _a$indeterminate = _a.indeterminate,
      indeterminate = _a$indeterminate === void 0 ? false : _a$indeterminate,
      style = _a.style,
      onMouseEnter = _a.onMouseEnter,
      onMouseLeave = _a.onMouseLeave,
      _a$skipGroup = _a.skipGroup,
      skipGroup = _a$skipGroup === void 0 ? false : _a$skipGroup,
      disabled = _a.disabled,
      restProps = __rest(_a, ["prefixCls", "className", "children", "indeterminate", "style", "onMouseEnter", "onMouseLeave", "skipGroup", "disabled"]);

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction;

  var checkboxGroup = React.useContext(_Group.GroupContext);

  var _useContext = (0, React.useContext)(_context.FormItemInputContext),
      isFormItemInput = _useContext.isFormItemInput;

  var contextDisabled = (0, React.useContext)(_DisabledContext["default"]);
  var mergedDisabled = disabled || (checkboxGroup === null || checkboxGroup === void 0 ? void 0 : checkboxGroup.disabled) || contextDisabled;
  var prevValue = React.useRef(restProps.value);
  React.useEffect(function () {
    checkboxGroup === null || checkboxGroup === void 0 ? void 0 : checkboxGroup.registerValue(restProps.value);
    process.env.NODE_ENV !== "production" ? (0, _warning["default"])('checked' in restProps || !!checkboxGroup || !('value' in restProps), 'Checkbox', '`value` is not a valid prop, do you mean `checked`?') : void 0;
  }, []);
  React.useEffect(function () {
    if (skipGroup) {
      return;
    }

    if (restProps.value !== prevValue.current) {
      checkboxGroup === null || checkboxGroup === void 0 ? void 0 : checkboxGroup.cancelValue(prevValue.current);
      checkboxGroup === null || checkboxGroup === void 0 ? void 0 : checkboxGroup.registerValue(restProps.value);
      prevValue.current = restProps.value;
    }

    return function () {
      return checkboxGroup === null || checkboxGroup === void 0 ? void 0 : checkboxGroup.cancelValue(restProps.value);
    };
  }, [restProps.value]);
  var prefixCls = getPrefixCls('checkbox', customizePrefixCls);
  var checkboxProps = (0, _extends2["default"])({}, restProps);

  if (checkboxGroup && !skipGroup) {
    checkboxProps.onChange = function () {
      if (restProps.onChange) {
        restProps.onChange.apply(restProps, arguments);
      }

      if (checkboxGroup.toggleOption) {
        checkboxGroup.toggleOption({
          label: children,
          value: restProps.value
        });
      }
    };

    checkboxProps.name = checkboxGroup.name;
    checkboxProps.checked = checkboxGroup.value.indexOf(restProps.value) !== -1;
  }

  var classString = (0, _classnames["default"])((_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-wrapper"), true), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-rtl"), direction === 'rtl'), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-wrapper-checked"), checkboxProps.checked), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-wrapper-disabled"), mergedDisabled), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-wrapper-in-form-item"), isFormItemInput), _classNames), className);
  var checkboxClass = (0, _classnames["default"])((0, _defineProperty2["default"])({}, "".concat(prefixCls, "-indeterminate"), indeterminate));
  var ariaChecked = indeterminate ? 'mixed' : undefined;
  return (
    /*#__PURE__*/
    // eslint-disable-next-line jsx-a11y/label-has-associated-control
    React.createElement("label", {
      className: classString,
      style: style,
      onMouseEnter: onMouseEnter,
      onMouseLeave: onMouseLeave
    }, /*#__PURE__*/React.createElement(_rcCheckbox["default"], (0, _extends2["default"])({
      "aria-checked": ariaChecked
    }, checkboxProps, {
      prefixCls: prefixCls,
      className: checkboxClass,
      disabled: mergedDisabled,
      ref: ref
    })), children !== undefined && /*#__PURE__*/React.createElement("span", null, children))
  );
};

var Checkbox = /*#__PURE__*/React.forwardRef(InternalCheckbox);

if (process.env.NODE_ENV !== 'production') {
  Checkbox.displayName = 'Checkbox';
}

var _default = Checkbox;
exports["default"] = _default;