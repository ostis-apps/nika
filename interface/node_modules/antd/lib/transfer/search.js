"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = Search;

var _SearchOutlined = _interopRequireDefault(require("@ant-design/icons/SearchOutlined"));

var React = _interopRequireWildcard(require("react"));

var _input = _interopRequireDefault(require("../input"));

function Search(props) {
  var _props$placeholder = props.placeholder,
      placeholder = _props$placeholder === void 0 ? '' : _props$placeholder,
      value = props.value,
      prefixCls = props.prefixCls,
      disabled = props.disabled,
      onChange = props.onChange,
      handleClear = props.handleClear;
  var handleChange = React.useCallback(function (e) {
    onChange === null || onChange === void 0 ? void 0 : onChange(e);

    if (e.target.value === '') {
      handleClear === null || handleClear === void 0 ? void 0 : handleClear();
    }
  }, [onChange]);
  return /*#__PURE__*/React.createElement(_input["default"], {
    placeholder: placeholder,
    className: prefixCls,
    value: value,
    onChange: handleChange,
    disabled: disabled,
    allowClear: true,
    prefix: /*#__PURE__*/React.createElement(_SearchOutlined["default"], null)
  });
}