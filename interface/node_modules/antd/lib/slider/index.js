"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _typeof2 = _interopRequireDefault(require("@babel/runtime/helpers/typeof"));

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _extends3 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _slicedToArray2 = _interopRequireDefault(require("@babel/runtime/helpers/slicedToArray"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcSlider = _interopRequireDefault(require("rc-slider"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _SliderTooltip = _interopRequireDefault(require("./SliderTooltip"));

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

var Slider = /*#__PURE__*/React.forwardRef(function (props, ref) {
  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction,
      getPopupContainer = _React$useContext.getPopupContainer;

  var _React$useState = React.useState({}),
      _React$useState2 = (0, _slicedToArray2["default"])(_React$useState, 2),
      visibles = _React$useState2[0],
      setVisibles = _React$useState2[1];

  var toggleTooltipVisible = function toggleTooltipVisible(index, visible) {
    setVisibles(function (prev) {
      return (0, _extends3["default"])((0, _extends3["default"])({}, prev), (0, _defineProperty2["default"])({}, index, visible));
    });
  };

  var getTooltipPlacement = function getTooltipPlacement(tooltipPlacement, vertical) {
    if (tooltipPlacement) {
      return tooltipPlacement;
    }

    if (!vertical) {
      return 'top';
    }

    return direction === 'rtl' ? 'left' : 'right';
  };

  var customizePrefixCls = props.prefixCls,
      customizeTooltipPrefixCls = props.tooltipPrefixCls,
      range = props.range,
      className = props.className,
      restProps = __rest(props, ["prefixCls", "tooltipPrefixCls", "range", "className"]);

  var prefixCls = getPrefixCls('slider', customizePrefixCls);
  var tooltipPrefixCls = getPrefixCls('tooltip', customizeTooltipPrefixCls);
  var cls = (0, _classnames["default"])(className, (0, _defineProperty2["default"])({}, "".concat(prefixCls, "-rtl"), direction === 'rtl')); // make reverse default on rtl direction

  if (direction === 'rtl' && !restProps.vertical) {
    restProps.reverse = !restProps.reverse;
  } // Range config


  var _React$useMemo = React.useMemo(function () {
    if (!range) {
      return [false];
    }

    return (0, _typeof2["default"])(range) === 'object' ? [true, range.draggableTrack] : [true, false];
  }, [range]),
      _React$useMemo2 = (0, _slicedToArray2["default"])(_React$useMemo, 2),
      mergedRange = _React$useMemo2[0],
      draggableTrack = _React$useMemo2[1];

  var handleRender = function handleRender(node, info) {
    var index = info.index,
        dragging = info.dragging;
    var rootPrefixCls = getPrefixCls();
    var tipFormatter = props.tipFormatter,
        tooltipVisible = props.tooltipVisible,
        tooltipPlacement = props.tooltipPlacement,
        getTooltipPopupContainer = props.getTooltipPopupContainer,
        vertical = props.vertical;
    var isTipFormatter = tipFormatter ? visibles[index] || dragging : false;
    var visible = tooltipVisible || tooltipVisible === undefined && isTipFormatter;
    var passedProps = (0, _extends3["default"])((0, _extends3["default"])({}, node.props), {
      onMouseEnter: function onMouseEnter() {
        return toggleTooltipVisible(index, true);
      },
      onMouseLeave: function onMouseLeave() {
        return toggleTooltipVisible(index, false);
      }
    });
    return /*#__PURE__*/React.createElement(_SliderTooltip["default"], {
      prefixCls: tooltipPrefixCls,
      title: tipFormatter ? tipFormatter(info.value) : '',
      visible: visible,
      placement: getTooltipPlacement(tooltipPlacement, vertical),
      transitionName: "".concat(rootPrefixCls, "-zoom-down"),
      key: index,
      overlayClassName: "".concat(prefixCls, "-tooltip"),
      getPopupContainer: getTooltipPopupContainer || getPopupContainer
    }, /*#__PURE__*/React.cloneElement(node, passedProps));
  };

  return /*#__PURE__*/React.createElement(_rcSlider["default"], (0, _extends3["default"])({}, restProps, {
    step: restProps.step,
    range: mergedRange,
    draggableTrack: draggableTrack,
    className: cls,
    ref: ref,
    prefixCls: prefixCls,
    handleRender: handleRender
  }));
});

if (process.env.NODE_ENV !== 'production') {
  Slider.displayName = 'Slider';
}

Slider.defaultProps = {
  tipFormatter: function tipFormatter(value) {
    return typeof value === 'number' ? value.toString() : '';
  }
};
var _default = Slider;
exports["default"] = _default;