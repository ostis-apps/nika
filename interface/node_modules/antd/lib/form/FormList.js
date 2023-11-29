"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _rcFieldForm = require("rc-field-form");

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _warning = _interopRequireDefault(require("../_util/warning"));

var _context = require("./context");

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

var FormList = function FormList(_a) {
  var customizePrefixCls = _a.prefixCls,
      children = _a.children,
      props = __rest(_a, ["prefixCls", "children"]);

  process.env.NODE_ENV !== "production" ? (0, _warning["default"])(!!props.name, 'Form.List', 'Miss `name` prop.') : void 0;

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls;

  var prefixCls = getPrefixCls('form', customizePrefixCls);
  var contextValue = React.useMemo(function () {
    return {
      prefixCls: prefixCls,
      status: 'error'
    };
  }, [prefixCls]);
  return /*#__PURE__*/React.createElement(_rcFieldForm.List, (0, _extends2["default"])({}, props), function (fields, operation, meta) {
    return /*#__PURE__*/React.createElement(_context.FormItemPrefixContext.Provider, {
      value: contextValue
    }, children(fields.map(function (field) {
      return (0, _extends2["default"])((0, _extends2["default"])({}, field), {
        fieldKey: field.key
      });
    }), operation, {
      errors: meta.errors,
      warnings: meta.warnings
    }));
  });
};

var _default = FormList;
exports["default"] = _default;