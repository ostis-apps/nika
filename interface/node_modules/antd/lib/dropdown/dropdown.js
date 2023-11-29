"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _typeof2 = _interopRequireDefault(require("@babel/runtime/helpers/typeof"));

var _slicedToArray2 = _interopRequireDefault(require("@babel/runtime/helpers/slicedToArray"));

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _RightOutlined = _interopRequireDefault(require("@ant-design/icons/RightOutlined"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcDropdown = _interopRequireDefault(require("rc-dropdown"));

var _useEvent = _interopRequireDefault(require("rc-util/lib/hooks/useEvent"));

var _useMergedState3 = _interopRequireDefault(require("rc-util/lib/hooks/useMergedState"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _OverrideContext = require("../menu/OverrideContext");

var _placements = _interopRequireDefault(require("../_util/placements"));

var _reactNode = require("../_util/reactNode");

var _type = require("../_util/type");

var _warning = _interopRequireDefault(require("../_util/warning"));

var _dropdownButton = _interopRequireDefault(require("./dropdown-button"));

var Placements = (0, _type.tuple)('topLeft', 'topCenter', 'topRight', 'bottomLeft', 'bottomCenter', 'bottomRight', 'top', 'bottom');

var Dropdown = function Dropdown(props) {
  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getContextPopupContainer = _React$useContext.getPopupContainer,
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction;

  var getTransitionName = function getTransitionName() {
    var rootPrefixCls = getPrefixCls();
    var _props$placement = props.placement,
        placement = _props$placement === void 0 ? '' : _props$placement,
        transitionName = props.transitionName;

    if (transitionName !== undefined) {
      return transitionName;
    }

    if (placement.indexOf('top') >= 0) {
      return "".concat(rootPrefixCls, "-slide-down");
    }

    return "".concat(rootPrefixCls, "-slide-up");
  };

  var getPlacement = function getPlacement() {
    var placement = props.placement;

    if (!placement) {
      return direction === 'rtl' ? 'bottomRight' : 'bottomLeft';
    }

    if (placement.includes('Center')) {
      var newPlacement = placement.slice(0, placement.indexOf('Center'));
      process.env.NODE_ENV !== "production" ? (0, _warning["default"])(!placement.includes('Center'), 'Dropdown', "You are using '".concat(placement, "' placement in Dropdown, which is deprecated. Try to use '").concat(newPlacement, "' instead.")) : void 0;
      return newPlacement;
    }

    return placement;
  };

  var arrow = props.arrow,
      customizePrefixCls = props.prefixCls,
      children = props.children,
      trigger = props.trigger,
      disabled = props.disabled,
      getPopupContainer = props.getPopupContainer,
      overlayClassName = props.overlayClassName,
      visible = props.visible,
      onVisibleChange = props.onVisibleChange;
  var prefixCls = getPrefixCls('dropdown', customizePrefixCls);
  var child = React.Children.only(children);
  var dropdownTrigger = (0, _reactNode.cloneElement)(child, {
    className: (0, _classnames["default"])("".concat(prefixCls, "-trigger"), (0, _defineProperty2["default"])({}, "".concat(prefixCls, "-rtl"), direction === 'rtl'), child.props.className),
    disabled: disabled
  });
  var triggerActions = disabled ? [] : trigger;
  var alignPoint;

  if (triggerActions && triggerActions.indexOf('contextMenu') !== -1) {
    alignPoint = true;
  } // =========================== Visible ============================


  var _useMergedState = (0, _useMergedState3["default"])(false, {
    value: visible
  }),
      _useMergedState2 = (0, _slicedToArray2["default"])(_useMergedState, 2),
      mergedVisible = _useMergedState2[0],
      setVisible = _useMergedState2[1];

  var onInnerVisibleChange = (0, _useEvent["default"])(function (nextVisible) {
    onVisibleChange === null || onVisibleChange === void 0 ? void 0 : onVisibleChange(nextVisible);
    setVisible(nextVisible);
  }); // =========================== Overlay ============================

  var overlayClassNameCustomized = (0, _classnames["default"])(overlayClassName, (0, _defineProperty2["default"])({}, "".concat(prefixCls, "-rtl"), direction === 'rtl'));
  var builtinPlacements = (0, _placements["default"])({
    arrowPointAtCenter: (0, _typeof2["default"])(arrow) === 'object' && arrow.pointAtCenter,
    autoAdjustOverflow: true
  });
  var onMenuClick = React.useCallback(function () {
    setVisible(false);
  }, []);

  var renderOverlay = function renderOverlay() {
    // rc-dropdown already can process the function of overlay, but we have check logic here.
    // So we need render the element to check and pass back to rc-dropdown.
    var overlay = props.overlay;
    var overlayNode;

    if (typeof overlay === 'function') {
      overlayNode = overlay();
    } else {
      overlayNode = overlay;
    }

    overlayNode = React.Children.only(typeof overlayNode === 'string' ? /*#__PURE__*/React.createElement("span", null, overlayNode) : overlayNode);
    return /*#__PURE__*/React.createElement(_OverrideContext.OverrideProvider, {
      prefixCls: "".concat(prefixCls, "-menu"),
      expandIcon: /*#__PURE__*/React.createElement("span", {
        className: "".concat(prefixCls, "-menu-submenu-arrow")
      }, /*#__PURE__*/React.createElement(_RightOutlined["default"], {
        className: "".concat(prefixCls, "-menu-submenu-arrow-icon")
      })),
      mode: "vertical",
      selectable: false,
      onClick: onMenuClick,
      validator: function validator(_ref) {
        var mode = _ref.mode;
        // Warning if use other mode
        process.env.NODE_ENV !== "production" ? (0, _warning["default"])(!mode || mode === 'vertical', 'Dropdown', "mode=\"".concat(mode, "\" is not supported for Dropdown's Menu.")) : void 0;
      }
    }, overlayNode);
  }; // ============================ Render ============================


  return /*#__PURE__*/React.createElement(_rcDropdown["default"], (0, _extends2["default"])({
    alignPoint: alignPoint
  }, props, {
    visible: mergedVisible,
    builtinPlacements: builtinPlacements,
    arrow: !!arrow,
    overlayClassName: overlayClassNameCustomized,
    prefixCls: prefixCls,
    getPopupContainer: getPopupContainer || getContextPopupContainer,
    transitionName: getTransitionName(),
    trigger: triggerActions,
    overlay: renderOverlay,
    placement: getPlacement(),
    onVisibleChange: onInnerVisibleChange
  }), dropdownTrigger);
};

Dropdown.Button = _dropdownButton["default"];
Dropdown.defaultProps = {
  mouseEnterDelay: 0.15,
  mouseLeaveDelay: 0.1
};
var _default = Dropdown;
exports["default"] = _default;