"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = getIcons;

var _CheckOutlined = _interopRequireDefault(require("@ant-design/icons/CheckOutlined"));

var _CloseCircleFilled = _interopRequireDefault(require("@ant-design/icons/CloseCircleFilled"));

var _CloseOutlined = _interopRequireDefault(require("@ant-design/icons/CloseOutlined"));

var _DownOutlined = _interopRequireDefault(require("@ant-design/icons/DownOutlined"));

var _LoadingOutlined = _interopRequireDefault(require("@ant-design/icons/LoadingOutlined"));

var _SearchOutlined = _interopRequireDefault(require("@ant-design/icons/SearchOutlined"));

var React = _interopRequireWildcard(require("react"));

function getIcons(_ref) {
  var suffixIcon = _ref.suffixIcon,
      clearIcon = _ref.clearIcon,
      menuItemSelectedIcon = _ref.menuItemSelectedIcon,
      removeIcon = _ref.removeIcon,
      loading = _ref.loading,
      multiple = _ref.multiple,
      hasFeedback = _ref.hasFeedback,
      prefixCls = _ref.prefixCls,
      showArrow = _ref.showArrow,
      feedbackIcon = _ref.feedbackIcon;
  // Clear Icon
  var mergedClearIcon = clearIcon;

  if (!clearIcon) {
    mergedClearIcon = /*#__PURE__*/React.createElement(_CloseCircleFilled["default"], null);
  } // Validation Feedback Icon


  var getSuffixIconNode = function getSuffixIconNode(arrowIcon) {
    return /*#__PURE__*/React.createElement(React.Fragment, null, showArrow !== false && arrowIcon, hasFeedback && feedbackIcon);
  }; // Arrow item icon


  var mergedSuffixIcon = null;

  if (suffixIcon !== undefined) {
    mergedSuffixIcon = getSuffixIconNode(suffixIcon);
  } else if (loading) {
    mergedSuffixIcon = getSuffixIconNode( /*#__PURE__*/React.createElement(_LoadingOutlined["default"], {
      spin: true
    }));
  } else {
    var iconCls = "".concat(prefixCls, "-suffix");

    mergedSuffixIcon = function mergedSuffixIcon(_ref2) {
      var open = _ref2.open,
          showSearch = _ref2.showSearch;

      if (open && showSearch) {
        return getSuffixIconNode( /*#__PURE__*/React.createElement(_SearchOutlined["default"], {
          className: iconCls
        }));
      }

      return getSuffixIconNode( /*#__PURE__*/React.createElement(_DownOutlined["default"], {
        className: iconCls
      }));
    };
  } // Checked item icon


  var mergedItemIcon = null;

  if (menuItemSelectedIcon !== undefined) {
    mergedItemIcon = menuItemSelectedIcon;
  } else if (multiple) {
    mergedItemIcon = /*#__PURE__*/React.createElement(_CheckOutlined["default"], null);
  } else {
    mergedItemIcon = null;
  }

  var mergedRemoveIcon = null;

  if (removeIcon !== undefined) {
    mergedRemoveIcon = removeIcon;
  } else {
    mergedRemoveIcon = /*#__PURE__*/React.createElement(_CloseOutlined["default"], null);
  }

  return {
    clearIcon: mergedClearIcon,
    suffixIcon: mergedSuffixIcon,
    itemIcon: mergedItemIcon,
    removeIcon: mergedRemoveIcon
  };
}