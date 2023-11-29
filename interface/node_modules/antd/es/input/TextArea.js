import _typeof from "@babel/runtime/helpers/esm/typeof";
import _defineProperty from "@babel/runtime/helpers/esm/defineProperty";
import _extends from "@babel/runtime/helpers/esm/extends";
import _slicedToArray from "@babel/runtime/helpers/esm/slicedToArray";
import _toConsumableArray from "@babel/runtime/helpers/esm/toConsumableArray";

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

import classNames from 'classnames';
import RcTextArea from 'rc-textarea';
import useMergedState from "rc-util/es/hooks/useMergedState";
import omit from "rc-util/es/omit";
import * as React from 'react';
import { ConfigContext } from '../config-provider';
import DisabledContext from '../config-provider/DisabledContext';
import SizeContext from '../config-provider/SizeContext';
import { FormItemInputContext } from '../form/context';
import { getMergedStatus, getStatusClassNames } from '../_util/statusUtils';
import ClearableLabeledInput from './ClearableLabeledInput';
import { fixControlledValue, resolveOnChange, triggerFocus } from './Input';

function fixEmojiLength(value, maxLength) {
  return _toConsumableArray(value || '').slice(0, maxLength).join('');
}

function setTriggerValue(isCursorInEnd, preValue, triggerValue, maxLength) {
  var newTriggerValue = triggerValue;

  if (isCursorInEnd) {
    // 光标在尾部，直接截断
    newTriggerValue = fixEmojiLength(triggerValue, maxLength);
  } else if (_toConsumableArray(preValue || '').length < triggerValue.length && _toConsumableArray(triggerValue || '').length > maxLength) {
    // 光标在中间，如果最后的值超过最大值，则采用原先的值
    newTriggerValue = preValue;
  }

  return newTriggerValue;
}

var TextArea = /*#__PURE__*/React.forwardRef(function (_a, ref) {
  var _classNames;

  var customizePrefixCls = _a.prefixCls,
      _a$bordered = _a.bordered,
      bordered = _a$bordered === void 0 ? true : _a$bordered,
      _a$showCount = _a.showCount,
      showCount = _a$showCount === void 0 ? false : _a$showCount,
      maxLength = _a.maxLength,
      className = _a.className,
      style = _a.style,
      customizeSize = _a.size,
      customDisabled = _a.disabled,
      onCompositionStart = _a.onCompositionStart,
      onCompositionEnd = _a.onCompositionEnd,
      onChange = _a.onChange,
      customStatus = _a.status,
      props = __rest(_a, ["prefixCls", "bordered", "showCount", "maxLength", "className", "style", "size", "disabled", "onCompositionStart", "onCompositionEnd", "onChange", "status"]);

  var _React$useContext = React.useContext(ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction;

  var size = React.useContext(SizeContext); // ===================== Disabled =====================

  var disabled = React.useContext(DisabledContext);
  var mergedDisabled = customDisabled || disabled;

  var _React$useContext2 = React.useContext(FormItemInputContext),
      contextStatus = _React$useContext2.status,
      hasFeedback = _React$useContext2.hasFeedback,
      isFormItemInput = _React$useContext2.isFormItemInput,
      feedbackIcon = _React$useContext2.feedbackIcon;

  var mergedStatus = getMergedStatus(contextStatus, customStatus);
  var innerRef = React.useRef(null);
  var clearableInputRef = React.useRef(null);

  var _React$useState = React.useState(false),
      _React$useState2 = _slicedToArray(_React$useState, 2),
      compositing = _React$useState2[0],
      setCompositing = _React$useState2[1];

  var oldCompositionValueRef = React.useRef();
  var oldSelectionStartRef = React.useRef(0);

  var _useMergedState = useMergedState(props.defaultValue, {
    value: props.value
  }),
      _useMergedState2 = _slicedToArray(_useMergedState, 2),
      value = _useMergedState2[0],
      setValue = _useMergedState2[1];

  var hidden = props.hidden;

  var handleSetValue = function handleSetValue(val, callback) {
    if (props.value === undefined) {
      setValue(val);
      callback === null || callback === void 0 ? void 0 : callback();
    }
  }; // =========================== Value Update ===========================
  // Max length value


  var hasMaxLength = Number(maxLength) > 0;

  var onInternalCompositionStart = function onInternalCompositionStart(e) {
    setCompositing(true); // 拼音输入前保存一份旧值

    oldCompositionValueRef.current = value; // 保存旧的光标位置

    oldSelectionStartRef.current = e.currentTarget.selectionStart;
    onCompositionStart === null || onCompositionStart === void 0 ? void 0 : onCompositionStart(e);
  };

  var onInternalCompositionEnd = function onInternalCompositionEnd(e) {
    var _a;

    setCompositing(false);
    var triggerValue = e.currentTarget.value;

    if (hasMaxLength) {
      var isCursorInEnd = oldSelectionStartRef.current >= maxLength + 1 || oldSelectionStartRef.current === ((_a = oldCompositionValueRef.current) === null || _a === void 0 ? void 0 : _a.length);
      triggerValue = setTriggerValue(isCursorInEnd, oldCompositionValueRef.current, triggerValue, maxLength);
    } // Patch composition onChange when value changed


    if (triggerValue !== value) {
      handleSetValue(triggerValue);
      resolveOnChange(e.currentTarget, e, onChange, triggerValue);
    }

    onCompositionEnd === null || onCompositionEnd === void 0 ? void 0 : onCompositionEnd(e);
  };

  var handleChange = function handleChange(e) {
    var triggerValue = e.target.value;

    if (!compositing && hasMaxLength) {
      // 1. 复制粘贴超过maxlength的情况 2.未超过maxlength的情况
      var isCursorInEnd = e.target.selectionStart >= maxLength + 1 || e.target.selectionStart === triggerValue.length || !e.target.selectionStart;
      triggerValue = setTriggerValue(isCursorInEnd, value, triggerValue, maxLength);
    }

    handleSetValue(triggerValue);
    resolveOnChange(e.currentTarget, e, onChange, triggerValue);
  }; // ============================== Reset ===============================


  var handleReset = function handleReset(e) {
    var _a, _b, _c;

    handleSetValue('');
    (_a = innerRef.current) === null || _a === void 0 ? void 0 : _a.focus();
    resolveOnChange((_c = (_b = innerRef.current) === null || _b === void 0 ? void 0 : _b.resizableTextArea) === null || _c === void 0 ? void 0 : _c.textArea, e, onChange);
  };

  var prefixCls = getPrefixCls('input', customizePrefixCls);
  React.useImperativeHandle(ref, function () {
    var _a;

    return {
      resizableTextArea: (_a = innerRef.current) === null || _a === void 0 ? void 0 : _a.resizableTextArea,
      focus: function focus(option) {
        var _a, _b;

        triggerFocus((_b = (_a = innerRef.current) === null || _a === void 0 ? void 0 : _a.resizableTextArea) === null || _b === void 0 ? void 0 : _b.textArea, option);
      },
      blur: function blur() {
        var _a;

        return (_a = innerRef.current) === null || _a === void 0 ? void 0 : _a.blur();
      }
    };
  });
  var textArea = /*#__PURE__*/React.createElement(RcTextArea, _extends({}, omit(props, ['allowClear']), {
    disabled: mergedDisabled,
    className: classNames((_classNames = {}, _defineProperty(_classNames, "".concat(prefixCls, "-borderless"), !bordered), _defineProperty(_classNames, className, className && !showCount), _defineProperty(_classNames, "".concat(prefixCls, "-sm"), size === 'small' || customizeSize === 'small'), _defineProperty(_classNames, "".concat(prefixCls, "-lg"), size === 'large' || customizeSize === 'large'), _classNames), getStatusClassNames(prefixCls, mergedStatus)),
    style: showCount ? undefined : style,
    prefixCls: prefixCls,
    onCompositionStart: onInternalCompositionStart,
    onChange: handleChange,
    onCompositionEnd: onInternalCompositionEnd,
    ref: innerRef
  }));
  var val = fixControlledValue(value);

  if (!compositing && hasMaxLength && (props.value === null || props.value === undefined)) {
    // fix #27612 将value转为数组进行截取，解决 '😂'.length === 2 等emoji表情导致的截取乱码的问题
    val = fixEmojiLength(val, maxLength);
  } // TextArea


  var textareaNode = /*#__PURE__*/React.createElement(ClearableLabeledInput, _extends({
    disabled: mergedDisabled
  }, props, {
    prefixCls: prefixCls,
    direction: direction,
    inputType: "text",
    value: val,
    element: textArea,
    handleReset: handleReset,
    ref: clearableInputRef,
    bordered: bordered,
    status: customStatus,
    style: showCount ? undefined : style
  })); // Only show text area wrapper when needed

  if (showCount || hasFeedback) {
    var _classNames2;

    var valueLength = _toConsumableArray(val).length;

    var dataCount = '';

    if (_typeof(showCount) === 'object') {
      dataCount = showCount.formatter({
        count: valueLength,
        maxLength: maxLength
      });
    } else {
      dataCount = "".concat(valueLength).concat(hasMaxLength ? " / ".concat(maxLength) : '');
    }

    return /*#__PURE__*/React.createElement("div", {
      hidden: hidden,
      className: classNames("".concat(prefixCls, "-textarea"), (_classNames2 = {}, _defineProperty(_classNames2, "".concat(prefixCls, "-textarea-rtl"), direction === 'rtl'), _defineProperty(_classNames2, "".concat(prefixCls, "-textarea-show-count"), showCount), _defineProperty(_classNames2, "".concat(prefixCls, "-textarea-in-form-item"), isFormItemInput), _classNames2), getStatusClassNames("".concat(prefixCls, "-textarea"), mergedStatus, hasFeedback), className),
      style: style,
      "data-count": dataCount
    }, textareaNode, hasFeedback && /*#__PURE__*/React.createElement("span", {
      className: "".concat(prefixCls, "-textarea-suffix")
    }, feedbackIcon));
  }

  return textareaNode;
});
export default TextArea;