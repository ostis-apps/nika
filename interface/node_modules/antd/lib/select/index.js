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

var _rcSelect = _interopRequireWildcard(require("rc-select"));

var _omit = _interopRequireDefault(require("rc-util/lib/omit"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _defaultRenderEmpty = _interopRequireDefault(require("../config-provider/defaultRenderEmpty"));

var _DisabledContext = _interopRequireDefault(require("../config-provider/DisabledContext"));

var _SizeContext = _interopRequireDefault(require("../config-provider/SizeContext"));

var _context = require("../form/context");

var _motion = require("../_util/motion");

var _statusUtils = require("../_util/statusUtils");

var _iconUtil = _interopRequireDefault(require("./utils/iconUtil"));

// TODO: 4.0 - codemod should help to change `filterOption` to support node props.
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

var SECRET_COMBOBOX_MODE_DO_NOT_USE = 'SECRET_COMBOBOX_MODE_DO_NOT_USE';

var InternalSelect = function InternalSelect(_a, ref) {
  var _classNames2;

  var customizePrefixCls = _a.prefixCls,
      _a$bordered = _a.bordered,
      bordered = _a$bordered === void 0 ? true : _a$bordered,
      className = _a.className,
      getPopupContainer = _a.getPopupContainer,
      dropdownClassName = _a.dropdownClassName,
      _a$listHeight = _a.listHeight,
      listHeight = _a$listHeight === void 0 ? 256 : _a$listHeight,
      placement = _a.placement,
      _a$listItemHeight = _a.listItemHeight,
      listItemHeight = _a$listItemHeight === void 0 ? 24 : _a$listItemHeight,
      customizeSize = _a.size,
      customDisabled = _a.disabled,
      notFoundContent = _a.notFoundContent,
      customStatus = _a.status,
      showArrow = _a.showArrow,
      props = __rest(_a, ["prefixCls", "bordered", "className", "getPopupContainer", "dropdownClassName", "listHeight", "placement", "listItemHeight", "size", "disabled", "notFoundContent", "status", "showArrow"]);

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getContextPopupContainer = _React$useContext.getPopupContainer,
      getPrefixCls = _React$useContext.getPrefixCls,
      renderEmpty = _React$useContext.renderEmpty,
      direction = _React$useContext.direction,
      virtual = _React$useContext.virtual,
      dropdownMatchSelectWidth = _React$useContext.dropdownMatchSelectWidth;

  var size = React.useContext(_SizeContext["default"]);
  var prefixCls = getPrefixCls('select', customizePrefixCls);
  var rootPrefixCls = getPrefixCls();
  var mode = React.useMemo(function () {
    var m = props.mode;

    if (m === 'combobox') {
      return undefined;
    }

    if (m === SECRET_COMBOBOX_MODE_DO_NOT_USE) {
      return 'combobox';
    }

    return m;
  }, [props.mode]);
  var isMultiple = mode === 'multiple' || mode === 'tags';
  var mergedShowArrow = showArrow !== undefined ? showArrow : props.loading || !(isMultiple || mode === 'combobox'); // ===================== Form Status =====================

  var _useContext = (0, React.useContext)(_context.FormItemInputContext),
      contextStatus = _useContext.status,
      hasFeedback = _useContext.hasFeedback,
      isFormItemInput = _useContext.isFormItemInput,
      feedbackIcon = _useContext.feedbackIcon;

  var mergedStatus = (0, _statusUtils.getMergedStatus)(contextStatus, customStatus); // ===================== Empty =====================

  var mergedNotFound;

  if (notFoundContent !== undefined) {
    mergedNotFound = notFoundContent;
  } else if (mode === 'combobox') {
    mergedNotFound = null;
  } else {
    mergedNotFound = (renderEmpty || _defaultRenderEmpty["default"])('Select');
  } // ===================== Icons =====================


  var _getIcons = (0, _iconUtil["default"])((0, _extends2["default"])((0, _extends2["default"])({}, props), {
    multiple: isMultiple,
    hasFeedback: hasFeedback,
    feedbackIcon: feedbackIcon,
    showArrow: mergedShowArrow,
    prefixCls: prefixCls
  })),
      suffixIcon = _getIcons.suffixIcon,
      itemIcon = _getIcons.itemIcon,
      removeIcon = _getIcons.removeIcon,
      clearIcon = _getIcons.clearIcon;

  var selectProps = (0, _omit["default"])(props, ['suffixIcon', 'itemIcon']);
  var rcSelectRtlDropdownClassName = (0, _classnames["default"])(dropdownClassName, (0, _defineProperty2["default"])({}, "".concat(prefixCls, "-dropdown-").concat(direction), direction === 'rtl'));
  var mergedSize = customizeSize || size; // ===================== Disabled =====================

  var disabled = React.useContext(_DisabledContext["default"]);
  var mergedDisabled = customDisabled || disabled;
  var mergedClassName = (0, _classnames["default"])((_classNames2 = {}, (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-lg"), mergedSize === 'large'), (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-sm"), mergedSize === 'small'), (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-rtl"), direction === 'rtl'), (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-borderless"), !bordered), (0, _defineProperty2["default"])(_classNames2, "".concat(prefixCls, "-in-form-item"), isFormItemInput), _classNames2), (0, _statusUtils.getStatusClassNames)(prefixCls, mergedStatus, hasFeedback), className); // ===================== Placement =====================

  var getPlacement = function getPlacement() {
    if (placement !== undefined) {
      return placement;
    }

    return direction === 'rtl' ? 'bottomRight' : 'bottomLeft';
  };

  return /*#__PURE__*/React.createElement(_rcSelect["default"], (0, _extends2["default"])({
    ref: ref,
    virtual: virtual,
    dropdownMatchSelectWidth: dropdownMatchSelectWidth
  }, selectProps, {
    transitionName: (0, _motion.getTransitionName)(rootPrefixCls, (0, _motion.getTransitionDirection)(placement), props.transitionName),
    listHeight: listHeight,
    listItemHeight: listItemHeight,
    mode: mode,
    prefixCls: prefixCls,
    placement: getPlacement(),
    direction: direction,
    inputIcon: suffixIcon,
    menuItemSelectedIcon: itemIcon,
    removeIcon: removeIcon,
    clearIcon: clearIcon,
    notFoundContent: mergedNotFound,
    className: mergedClassName,
    getPopupContainer: getPopupContainer || getContextPopupContainer,
    dropdownClassName: rcSelectRtlDropdownClassName,
    showArrow: hasFeedback || showArrow,
    disabled: mergedDisabled
  }));
};

var Select = /*#__PURE__*/React.forwardRef(InternalSelect);
Select.SECRET_COMBOBOX_MODE_DO_NOT_USE = SECRET_COMBOBOX_MODE_DO_NOT_USE;
Select.Option = _rcSelect.Option;
Select.OptGroup = _rcSelect.OptGroup;
var _default = Select;
exports["default"] = _default;