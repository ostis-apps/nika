import _defineProperty from "@babel/runtime/helpers/esm/defineProperty";
import _extends from "@babel/runtime/helpers/esm/extends";
import _typeof from "@babel/runtime/helpers/esm/typeof";

var __rest = this && this.__rest || function (s, e) {
  var t = {};

  for (var p in s) {
    if (Object.prototype.hasOwnProperty.call(s, p) && e.indexOf(p) < 0) t[p] = s[p];
  }

  if (s != null && typeof Object.getOwnPropertySymbols === "function") for (var i = 0, p = Object.getOwnPropertySymbols(s); i < p.length; i++) {
    if (e.indexOf(p[i]) < 0 && Object.prototype.propertyIsEnumerable.call(s, p[i])) t[p[i]] = s[p[i]];
  }
  return t;
};

import CloseCircleFilled from "@ant-design/icons/es/icons/CloseCircleFilled";
import classNames from 'classnames';
import RcInput from 'rc-input';
import { composeRef } from "rc-util/es/ref";
import React, { forwardRef, useContext, useEffect, useRef } from 'react';
import { ConfigContext } from '../config-provider';
import DisabledContext from '../config-provider/DisabledContext';
import SizeContext from '../config-provider/SizeContext';
import { FormItemInputContext, NoFormStyle } from '../form/context';
import { getMergedStatus, getStatusClassNames } from '../_util/statusUtils';
import warning from '../_util/warning';
import { hasPrefixSuffix } from './utils';
export function fixControlledValue(value) {
  if (typeof value === 'undefined' || value === null) {
    return '';
  }

  return String(value);
}
export function resolveOnChange(target, e, onChange, targetValue) {
  if (!onChange) {
    return;
  }

  var event = e;

  if (e.type === 'click') {
    // Clone a new target for event.
    // Avoid the following usage, the setQuery method gets the original value.
    //
    // const [query, setQuery] = React.useState('');
    // <Input
    //   allowClear
    //   value={query}
    //   onChange={(e)=> {
    //     setQuery((prevStatus) => e.target.value);
    //   }}
    // />
    var currentTarget = target.cloneNode(true); // click clear icon

    event = Object.create(e, {
      target: {
        value: currentTarget
      },
      currentTarget: {
        value: currentTarget
      }
    });
    currentTarget.value = '';
    onChange(event);
    return;
  } // Trigger by composition event, this means we need force change the input value


  if (targetValue !== undefined) {
    event = Object.create(e, {
      target: {
        value: target
      },
      currentTarget: {
        value: target
      }
    });
    target.value = targetValue;
    onChange(event);
    return;
  }

  onChange(event);
}
export function triggerFocus(element, option) {
  if (!element) return;
  element.focus(option); // Selection content

  var _ref = option || {},
      cursor = _ref.cursor;

  if (cursor) {
    var len = element.value.length;

    switch (cursor) {
      case 'start':
        element.setSelectionRange(0, 0);
        break;

      case 'end':
        element.setSelectionRange(len, len);
        break;

      default:
        element.setSelectionRange(0, len);
    }
  }
}
var Input = /*#__PURE__*/forwardRef(function (props, ref) {
  var _classNames, _classNames2, _classNames4;

  var customizePrefixCls = props.prefixCls,
      _props$bordered = props.bordered,
      bordered = _props$bordered === void 0 ? true : _props$bordered,
      customStatus = props.status,
      customSize = props.size,
      customDisabled = props.disabled,
      onBlur = props.onBlur,
      onFocus = props.onFocus,
      suffix = props.suffix,
      allowClear = props.allowClear,
      addonAfter = props.addonAfter,
      addonBefore = props.addonBefore,
      rest = __rest(props, ["prefixCls", "bordered", "status", "size", "disabled", "onBlur", "onFocus", "suffix", "allowClear", "addonAfter", "addonBefore"]);

  var _React$useContext = React.useContext(ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction,
      input = _React$useContext.input;

  var prefixCls = getPrefixCls('input', customizePrefixCls);
  var inputRef = useRef(null); // ===================== Size =====================

  var size = React.useContext(SizeContext);
  var mergedSize = customSize || size; // ===================== Disabled =====================

  var disabled = React.useContext(DisabledContext);
  var mergedDisabled = customDisabled || disabled; // ===================== Status =====================

  var _useContext = useContext(FormItemInputContext),
      contextStatus = _useContext.status,
      hasFeedback = _useContext.hasFeedback,
      feedbackIcon = _useContext.feedbackIcon;

  var mergedStatus = getMergedStatus(contextStatus, customStatus); // ===================== Focus warning =====================

  var inputHasPrefixSuffix = hasPrefixSuffix(props) || !!hasFeedback;
  var prevHasPrefixSuffix = useRef(inputHasPrefixSuffix);
  useEffect(function () {
    var _a;

    if (inputHasPrefixSuffix && !prevHasPrefixSuffix.current) {
      process.env.NODE_ENV !== "production" ? warning(document.activeElement === ((_a = inputRef.current) === null || _a === void 0 ? void 0 : _a.input), 'Input', "When Input is focused, dynamic add or remove prefix / suffix will make it lose focus caused by dom structure change. Read more: https://ant.design/components/input/#FAQ") : void 0;
    }

    prevHasPrefixSuffix.current = inputHasPrefixSuffix;
  }, [inputHasPrefixSuffix]); // ===================== Remove Password value =====================

  var removePasswordTimeoutRef = useRef([]);

  var removePasswordTimeout = function removePasswordTimeout() {
    removePasswordTimeoutRef.current.push(window.setTimeout(function () {
      var _a, _b, _c, _d;

      if (((_a = inputRef.current) === null || _a === void 0 ? void 0 : _a.input) && ((_b = inputRef.current) === null || _b === void 0 ? void 0 : _b.input.getAttribute('type')) === 'password' && ((_c = inputRef.current) === null || _c === void 0 ? void 0 : _c.input.hasAttribute('value'))) {
        (_d = inputRef.current) === null || _d === void 0 ? void 0 : _d.input.removeAttribute('value');
      }
    }));
  };

  useEffect(function () {
    removePasswordTimeout();
    return function () {
      return removePasswordTimeoutRef.current.forEach(function (item) {
        return window.clearTimeout(item);
      });
    };
  }, []);

  var handleBlur = function handleBlur(e) {
    removePasswordTimeout();
    onBlur === null || onBlur === void 0 ? void 0 : onBlur(e);
  };

  var handleFocus = function handleFocus(e) {
    removePasswordTimeout();
    onFocus === null || onFocus === void 0 ? void 0 : onFocus(e);
  };

  var suffixNode = (hasFeedback || suffix) && /*#__PURE__*/React.createElement(React.Fragment, null, suffix, hasFeedback && feedbackIcon); // Allow clear

  var mergedAllowClear;

  if (_typeof(allowClear) === 'object' && (allowClear === null || allowClear === void 0 ? void 0 : allowClear.clearIcon)) {
    mergedAllowClear = allowClear;
  } else if (allowClear) {
    mergedAllowClear = {
      clearIcon: /*#__PURE__*/React.createElement(CloseCircleFilled, null)
    };
  }

  return /*#__PURE__*/React.createElement(RcInput, _extends({
    ref: composeRef(ref, inputRef),
    prefixCls: prefixCls,
    autoComplete: input === null || input === void 0 ? void 0 : input.autoComplete
  }, rest, {
    disabled: mergedDisabled || undefined,
    onBlur: handleBlur,
    onFocus: handleFocus,
    suffix: suffixNode,
    allowClear: mergedAllowClear,
    addonAfter: addonAfter && /*#__PURE__*/React.createElement(NoFormStyle, {
      override: true,
      status: true
    }, addonAfter),
    addonBefore: addonBefore && /*#__PURE__*/React.createElement(NoFormStyle, {
      override: true,
      status: true
    }, addonBefore),
    inputClassName: classNames((_classNames = {}, _defineProperty(_classNames, "".concat(prefixCls, "-sm"), mergedSize === 'small'), _defineProperty(_classNames, "".concat(prefixCls, "-lg"), mergedSize === 'large'), _defineProperty(_classNames, "".concat(prefixCls, "-rtl"), direction === 'rtl'), _defineProperty(_classNames, "".concat(prefixCls, "-borderless"), !bordered), _classNames), !inputHasPrefixSuffix && getStatusClassNames(prefixCls, mergedStatus)),
    affixWrapperClassName: classNames((_classNames2 = {}, _defineProperty(_classNames2, "".concat(prefixCls, "-affix-wrapper-sm"), mergedSize === 'small'), _defineProperty(_classNames2, "".concat(prefixCls, "-affix-wrapper-lg"), mergedSize === 'large'), _defineProperty(_classNames2, "".concat(prefixCls, "-affix-wrapper-rtl"), direction === 'rtl'), _defineProperty(_classNames2, "".concat(prefixCls, "-affix-wrapper-borderless"), !bordered), _classNames2), getStatusClassNames("".concat(prefixCls, "-affix-wrapper"), mergedStatus, hasFeedback)),
    wrapperClassName: classNames(_defineProperty({}, "".concat(prefixCls, "-group-rtl"), direction === 'rtl')),
    groupClassName: classNames((_classNames4 = {}, _defineProperty(_classNames4, "".concat(prefixCls, "-group-wrapper-sm"), mergedSize === 'small'), _defineProperty(_classNames4, "".concat(prefixCls, "-group-wrapper-lg"), mergedSize === 'large'), _defineProperty(_classNames4, "".concat(prefixCls, "-group-wrapper-rtl"), direction === 'rtl'), _classNames4), getStatusClassNames("".concat(prefixCls, "-group-wrapper"), mergedStatus, hasFeedback))
  }));
});
export default Input;