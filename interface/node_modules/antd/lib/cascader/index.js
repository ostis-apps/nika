"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _typeof2 = _interopRequireDefault(require("@babel/runtime/helpers/typeof"));

var _toConsumableArray2 = _interopRequireDefault(require("@babel/runtime/helpers/toConsumableArray"));

var _LeftOutlined = _interopRequireDefault(require("@ant-design/icons/LeftOutlined"));

var _LoadingOutlined = _interopRequireDefault(require("@ant-design/icons/LoadingOutlined"));

var _RightOutlined = _interopRequireDefault(require("@ant-design/icons/RightOutlined"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcCascader = _interopRequireDefault(require("rc-cascader"));

var _omit = _interopRequireDefault(require("rc-util/lib/omit"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _defaultRenderEmpty = _interopRequireDefault(require("../config-provider/defaultRenderEmpty"));

var _DisabledContext = _interopRequireDefault(require("../config-provider/DisabledContext"));

var _SizeContext = _interopRequireDefault(require("../config-provider/SizeContext"));

var _context = require("../form/context");

var _iconUtil = _interopRequireDefault(require("../select/utils/iconUtil"));

var _motion = require("../_util/motion");

var _statusUtils = require("../_util/statusUtils");

var _warning = _interopRequireDefault(require("../_util/warning"));

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

var SHOW_CHILD = _rcCascader["default"].SHOW_CHILD,
    SHOW_PARENT = _rcCascader["default"].SHOW_PARENT;

function highlightKeyword(str, lowerKeyword, prefixCls) {
  var cells = str.toLowerCase().split(lowerKeyword).reduce(function (list, cur, index) {
    return index === 0 ? [cur] : [].concat((0, _toConsumableArray2["default"])(list), [lowerKeyword, cur]);
  }, []);
  var fillCells = [];
  var start = 0;
  cells.forEach(function (cell, index) {
    var end = start + cell.length;
    var originWorld = str.slice(start, end);
    start = end;

    if (index % 2 === 1) {
      originWorld =
      /*#__PURE__*/
      // eslint-disable-next-line react/no-array-index-key
      React.createElement("span", {
        className: "".concat(prefixCls, "-menu-item-keyword"),
        key: "seperator-".concat(index)
      }, originWorld);
    }

    fillCells.push(originWorld);
  });
  return fillCells;
}

var defaultSearchRender = function defaultSearchRender(inputValue, path, prefixCls, fieldNames) {
  var optionList = []; // We do lower here to save perf

  var lower = inputValue.toLowerCase();
  path.forEach(function (node, index) {
    if (index !== 0) {
      optionList.push(' / ');
    }

    var label = node[fieldNames.label];
    var type = (0, _typeof2["default"])(label);

    if (type === 'string' || type === 'number') {
      label = highlightKeyword(String(label), lower, prefixCls);
    }

    optionList.push(label);
  });
  return optionList;
};

var Cascader = /*#__PURE__*/React.forwardRef(function (props, ref) {
  var _classNames2;

  var customizePrefixCls = props.prefixCls,
      customizeSize = props.size,
      customDisabled = props.disabled,
      className = props.className,
      multiple = props.multiple,
      _props$bordered = props.bordered,
      bordered = _props$bordered === void 0 ? true : _props$bordered,
      transitionName = props.transitionName,
      _props$choiceTransiti = props.choiceTransitionName,
      choiceTransitionName = _props$choiceTransiti === void 0 ? '' : _props$choiceTransiti,
      popupClassName = props.popupClassName,
      dropdownClassName = props.dropdownClassName,
      expandIcon = props.expandIcon,
      placement = props.placement,
      showSearch = props.showSearch,
      _props$allowClear = props.allowClear,
      allowClear = _props$allowClear === void 0 ? true : _props$allowClear,
      notFoundContent = props.notFoundContent,
      direction = props.direction,
      getPopupContainer = props.getPopupContainer,
      customStatus = props.status,
      showArrow = props.showArrow,
      rest = __rest(props, ["prefixCls", "size", "disabled", "className", "multiple", "bordered", "transitionName", "choiceTransitionName", "popupClassName", "dropdownClassName", "expandIcon", "placement", "showSearch", "allowClear", "notFoundContent", "direction", "getPopupContainer", "status", "showArrow"]);

  var restProps = (0, _omit["default"])(rest, ['suffixIcon']);

  var _useContext = (0, React.useContext)(_configProvider.ConfigContext),
      getContextPopupContainer = _useContext.getPopupContainer,
      getPrefixCls = _useContext.getPrefixCls,
      renderEmpty = _useContext.renderEmpty,
      rootDirection = _useContext.direction;

  var mergedDirection = direction || rootDirection;
  var isRtl = mergedDirection === 'rtl'; // =================== Form =====================

  var _useContext2 = (0, React.useContext)(_context.FormItemInputContext),
      contextStatus = _useContext2.status,
      hasFeedback = _useContext2.hasFeedback,
      isFormItemInput = _useContext2.isFormItemInput,
      feedbackIcon = _useContext2.feedbackIcon;

  var mergedStatus = (0, _statusUtils.getMergedStatus)(contextStatus, customStatus); // =================== Warning =====================

  process.env.NODE_ENV !== "production" ? (0, _warning["default"])(popupClassName === undefined, 'Cascader', '`popupClassName` is deprecated. Please use `dropdownClassName` instead.') : void 0;
  process.env.NODE_ENV !== "production" ? (0, _warning["default"])(!multiple || !props.displayRender, 'Cascader', '`displayRender` not work on `multiple`. Please use `tagRender` instead.') : void 0; // =================== No Found ====================

  var mergedNotFoundContent = notFoundContent || (renderEmpty || _defaultRenderEmpty["default"])('Cascader'); // ==================== Prefix =====================


  var rootPrefixCls = getPrefixCls();
  var prefixCls = getPrefixCls('select', customizePrefixCls);
  var cascaderPrefixCls = getPrefixCls('cascader', customizePrefixCls); // =================== Dropdown ====================

  var mergedDropdownClassName = (0, _classnames["default"])(dropdownClassName || popupClassName, "".concat(cascaderPrefixCls, "-dropdown"), (0, _defineProperty2["default"])({}, "".concat(cascaderPrefixCls, "-dropdown-rtl"), mergedDirection === 'rtl')); // ==================== Search =====================

  var mergedShowSearch = React.useMemo(function () {
    if (!showSearch) {
      return showSearch;
    }

    var searchConfig = {
      render: defaultSearchRender
    };

    if ((0, _typeof2["default"])(showSearch) === 'object') {
      searchConfig = (0, _extends2["default"])((0, _extends2["default"])({}, searchConfig), showSearch);
    }

    return searchConfig;
  }, [showSearch]); // ===================== Size ======================

  var size = React.useContext(_SizeContext["default"]);
  var mergedSize = customizeSize || size; // ===================== Disabled =====================

  var disabled = React.useContext(_DisabledContext["default"]);
  var mergedDisabled = customDisabled || disabled; // ===================== Icon ======================

  var mergedExpandIcon = expandIcon;

  if (!expandIcon) {
    mergedExpandIcon = isRtl ? /*#__PURE__*/React.createElement(_LeftOutlined["default"], null) : /*#__PURE__*/React.createElement(_RightOutlined["default"], null);
  }

  var loadingIcon = /*#__PURE__*/React.createElement("span", {
    className: "".concat(prefixCls, "-menu-item-loading-icon")
  }, /*#__PURE__*/React.createElement(_LoadingOutlined["default"], {
    spin: true
  })); // =================== Multiple ====================

  var checkable = React.useMemo(function () {
    return multiple ? /*#__PURE__*/React.createElement("span", {
      className: "".concat(cascaderPrefixCls, "-checkbox-inner")
    }) : false;
  }, [multiple]); // ===================== Icons =====================

  var mergedShowArrow = showArrow !== undefined ? showArrow : props.loading || !multiple;

  var _getIcons = (0, _iconUtil["default"])((0, _extends2["default"])((0, _extends2["default"])({}, props), {
    hasFeedback: hasFeedback,
    feedbackIcon: feedbackIcon,
    showArrow: mergedShowArrow,
    multiple: multiple,
    prefixCls: prefixCls
  })),
      suffixIcon = _getIcons.suffixIcon,
      removeIcon = _getIcons.removeIcon,
      clearIcon = _getIcons.clearIcon; // ===================== Placement =====================


  var getPlacement = function getPlacement() {
    if (placement !== undefined) {
      return placement;
    }

    return direction === 'rtl' ? 'bottomRight' : 'bottomLeft';
  }; // ==================== Render =====================


  return /*#__PURE__*/React.createElement(_rcCascader["default"], (0, _extends2["default"])({
    prefixCls: prefixCls,
    className: (0, _classnames["default"])(!customizePrefixCls && cascaderPrefixCls, (_classNames2 = {}, (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-lg"), mergedSize === 'large'), (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-sm"), mergedSize === 'small'), (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-rtl"), isRtl), (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-borderless"), !bordered), (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-in-form-item"), isFormItemInput), _classNames2), (0, _statusUtils.getStatusClassNames)(prefixCls, mergedStatus, hasFeedback), className),
    disabled: mergedDisabled
  }, restProps, {
    direction: mergedDirection,
    placement: getPlacement(),
    notFoundContent: mergedNotFoundContent,
    allowClear: allowClear,
    showSearch: mergedShowSearch,
    expandIcon: mergedExpandIcon,
    inputIcon: suffixIcon,
    removeIcon: removeIcon,
    clearIcon: clearIcon,
    loadingIcon: loadingIcon,
    checkable: checkable,
    dropdownClassName: mergedDropdownClassName,
    dropdownPrefixCls: customizePrefixCls || cascaderPrefixCls,
    choiceTransitionName: (0, _motion.getTransitionName)(rootPrefixCls, '', choiceTransitionName),
    transitionName: (0, _motion.getTransitionName)(rootPrefixCls, (0, _motion.getTransitionDirection)(placement), transitionName),
    getPopupContainer: getPopupContainer || getContextPopupContainer,
    ref: ref,
    showArrow: hasFeedback || showArrow
  }));
});

if (process.env.NODE_ENV !== 'production') {
  Cascader.displayName = 'Cascader';
}

Cascader.SHOW_PARENT = SHOW_PARENT;
Cascader.SHOW_CHILD = SHOW_CHILD;
var _default = Cascader;
exports["default"] = _default;