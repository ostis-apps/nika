"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _classCallCheck2 = _interopRequireDefault(require("@babel/runtime/helpers/classCallCheck"));

var _createClass2 = _interopRequireDefault(require("@babel/runtime/helpers/createClass"));

var _inherits2 = _interopRequireDefault(require("@babel/runtime/helpers/inherits"));

var _createSuper2 = _interopRequireDefault(require("@babel/runtime/helpers/createSuper"));

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _EllipsisOutlined = _interopRequireDefault(require("@ant-design/icons/EllipsisOutlined"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcMenu = _interopRequireWildcard(require("rc-menu"));

var _useEvent = _interopRequireDefault(require("rc-util/lib/hooks/useEvent"));

var _omit = _interopRequireDefault(require("rc-util/lib/omit"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _Sider = require("../layout/Sider");

var _motion = _interopRequireDefault(require("../_util/motion"));

var _reactNode = require("../_util/reactNode");

var _warning = _interopRequireDefault(require("../_util/warning"));

var _useItems = _interopRequireDefault(require("./hooks/useItems"));

var _MenuContext = _interopRequireDefault(require("./MenuContext"));

var _MenuDivider = _interopRequireDefault(require("./MenuDivider"));

var _MenuItem = _interopRequireDefault(require("./MenuItem"));

var _OverrideContext = _interopRequireDefault(require("./OverrideContext"));

var _SubMenu = _interopRequireDefault(require("./SubMenu"));

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

var InternalMenu = /*#__PURE__*/(0, React.forwardRef)(function (props, ref) {
  var _a;

  var override = React.useContext(_OverrideContext["default"]) || {};

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      getPopupContainer = _React$useContext.getPopupContainer,
      direction = _React$useContext.direction;

  var rootPrefixCls = getPrefixCls();

  var customizePrefixCls = props.prefixCls,
      className = props.className,
      _props$theme = props.theme,
      theme = _props$theme === void 0 ? 'light' : _props$theme,
      expandIcon = props.expandIcon,
      _internalDisableMenuItemTitleTooltip = props._internalDisableMenuItemTitleTooltip,
      inlineCollapsed = props.inlineCollapsed,
      siderCollapsed = props.siderCollapsed,
      items = props.items,
      children = props.children,
      mode = props.mode,
      selectable = props.selectable,
      onClick = props.onClick,
      restProps = __rest(props, ["prefixCls", "className", "theme", "expandIcon", "_internalDisableMenuItemTitleTooltip", "inlineCollapsed", "siderCollapsed", "items", "children", "mode", "selectable", "onClick"]);

  var passedProps = (0, _omit["default"])(restProps, ['collapsedWidth']); // ========================= Items ===========================

  var mergedChildren = (0, _useItems["default"])(items) || children; // ======================== Warning ==========================

  process.env.NODE_ENV !== "production" ? (0, _warning["default"])(!('inlineCollapsed' in props && mode !== 'inline'), 'Menu', '`inlineCollapsed` should only be used when `mode` is inline.') : void 0;
  process.env.NODE_ENV !== "production" ? (0, _warning["default"])(!(props.siderCollapsed !== undefined && 'inlineCollapsed' in props), 'Menu', '`inlineCollapsed` not control Menu under Sider. Should set `collapsed` on Sider instead.') : void 0;
  process.env.NODE_ENV !== "production" ? (0, _warning["default"])('items' in props && !children, 'Menu', '`children` will be removed in next major version. Please use `items` instead.') : void 0;
  (_a = override.validator) === null || _a === void 0 ? void 0 : _a.call(override, {
    mode: mode
  }); // ========================== Click ==========================
  // Tell dropdown that item clicked

  var onItemClick = (0, _useEvent["default"])(function () {
    var _a;

    onClick === null || onClick === void 0 ? void 0 : onClick.apply(void 0, arguments);
    (_a = override === null || override === void 0 ? void 0 : override.onClick) === null || _a === void 0 ? void 0 : _a.call(override);
  }); // ========================== Mode ===========================

  var mergedMode = override.mode || mode; // ======================= Selectable ========================

  var mergedSelectable = selectable !== null && selectable !== void 0 ? selectable : override.selectable; // ======================== Collapsed ========================
  // Inline Collapsed

  var mergedInlineCollapsed = React.useMemo(function () {
    if (siderCollapsed !== undefined) {
      return siderCollapsed;
    }

    return inlineCollapsed;
  }, [inlineCollapsed, siderCollapsed]);
  var defaultMotions = {
    horizontal: {
      motionName: "".concat(rootPrefixCls, "-slide-up")
    },
    inline: _motion["default"],
    other: {
      motionName: "".concat(rootPrefixCls, "-zoom-big")
    }
  };
  var prefixCls = getPrefixCls('menu', customizePrefixCls || override.prefixCls);
  var menuClassName = (0, _classnames["default"])("".concat(prefixCls, "-").concat(theme), className); // ====================== Expand Icon ========================

  var mergedExpandIcon;

  if (typeof expandIcon === 'function') {
    mergedExpandIcon = expandIcon;
  } else {
    mergedExpandIcon = (0, _reactNode.cloneElement)(expandIcon || override.expandIcon, {
      className: "".concat(prefixCls, "-submenu-expand-icon")
    });
  } // ======================== Context ==========================


  var contextValue = React.useMemo(function () {
    return {
      prefixCls: prefixCls,
      inlineCollapsed: mergedInlineCollapsed || false,
      antdMenuTheme: theme,
      direction: direction,
      firstLevel: true,
      disableMenuItemTitleTooltip: _internalDisableMenuItemTitleTooltip
    };
  }, [prefixCls, mergedInlineCollapsed, theme, direction, _internalDisableMenuItemTitleTooltip]); // ========================= Render ==========================

  return /*#__PURE__*/React.createElement(_OverrideContext["default"].Provider, {
    value: null
  }, /*#__PURE__*/React.createElement(_MenuContext["default"].Provider, {
    value: contextValue
  }, /*#__PURE__*/React.createElement(_rcMenu["default"], (0, _extends2["default"])({
    getPopupContainer: getPopupContainer,
    overflowedIndicator: /*#__PURE__*/React.createElement(_EllipsisOutlined["default"], null),
    overflowedIndicatorPopupClassName: "".concat(prefixCls, "-").concat(theme),
    mode: mergedMode,
    selectable: mergedSelectable,
    onClick: onItemClick
  }, passedProps, {
    inlineCollapsed: mergedInlineCollapsed,
    className: menuClassName,
    prefixCls: prefixCls,
    direction: direction,
    defaultMotions: defaultMotions,
    expandIcon: mergedExpandIcon,
    ref: ref
  }), mergedChildren)));
}); // We should keep this as ref-able

var Menu = /*#__PURE__*/function (_React$Component) {
  (0, _inherits2["default"])(Menu, _React$Component);

  var _super = (0, _createSuper2["default"])(Menu);

  function Menu() {
    var _this;

    (0, _classCallCheck2["default"])(this, Menu);
    _this = _super.apply(this, arguments);

    _this.focus = function (options) {
      var _a;

      (_a = _this.menu) === null || _a === void 0 ? void 0 : _a.focus(options);
    };

    return _this;
  }

  (0, _createClass2["default"])(Menu, [{
    key: "render",
    value: function render() {
      var _this2 = this;

      return /*#__PURE__*/React.createElement(_Sider.SiderContext.Consumer, null, function (context) {
        return /*#__PURE__*/React.createElement(InternalMenu, (0, _extends2["default"])({
          ref: function ref(node) {
            _this2.menu = node;
          }
        }, _this2.props, context));
      });
    }
  }]);
  return Menu;
}(React.Component);

Menu.Divider = _MenuDivider["default"];
Menu.Item = _MenuItem["default"];
Menu.SubMenu = _SubMenu["default"];
Menu.ItemGroup = _rcMenu.ItemGroup;
var _default = Menu;
exports["default"] = _default;