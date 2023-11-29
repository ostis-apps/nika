"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _SearchOutlined = _interopRequireDefault(require("@ant-design/icons/SearchOutlined"));

var React = _interopRequireWildcard(require("react"));

var _input = _interopRequireDefault(require("../../../input"));

var FilterSearch = function FilterSearch(_ref) {
  var value = _ref.value,
      onChange = _ref.onChange,
      filterSearch = _ref.filterSearch,
      tablePrefixCls = _ref.tablePrefixCls,
      locale = _ref.locale;

  if (!filterSearch) {
    return null;
  }

  return /*#__PURE__*/React.createElement("div", {
    className: "".concat(tablePrefixCls, "-filter-dropdown-search")
  }, /*#__PURE__*/React.createElement(_input["default"], {
    prefix: /*#__PURE__*/React.createElement(_SearchOutlined["default"], null),
    placeholder: locale.filterSearchPlaceholder,
    onChange: onChange,
    value: value,
    // for skip min-width of input
    htmlSize: 1,
    className: "".concat(tablePrefixCls, "-filter-dropdown-search-input")
  }));
};

var _default = FilterSearch;
exports["default"] = _default;