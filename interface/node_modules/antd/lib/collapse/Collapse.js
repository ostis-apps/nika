"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _RightOutlined = _interopRequireDefault(require("@ant-design/icons/RightOutlined"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcCollapse = _interopRequireDefault(require("rc-collapse"));

var React = _interopRequireWildcard(require("react"));

var _toArray = _interopRequireDefault(require("rc-util/lib/Children/toArray"));

var _omit = _interopRequireDefault(require("rc-util/lib/omit"));

var _configProvider = require("../config-provider");

var _motion = _interopRequireDefault(require("../_util/motion"));

var _reactNode = require("../_util/reactNode");

var _warning = _interopRequireDefault(require("../_util/warning"));

var _CollapsePanel = _interopRequireDefault(require("./CollapsePanel"));

var Collapse = function Collapse(props) {
  var _classNames;

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction;

  var customizePrefixCls = props.prefixCls,
      _props$className = props.className,
      className = _props$className === void 0 ? '' : _props$className,
      _props$bordered = props.bordered,
      bordered = _props$bordered === void 0 ? true : _props$bordered,
      ghost = props.ghost,
      _props$expandIconPosi = props.expandIconPosition,
      expandIconPosition = _props$expandIconPosi === void 0 ? 'start' : _props$expandIconPosi;
  var prefixCls = getPrefixCls('collapse', customizePrefixCls); // Warning if use legacy type `expandIconPosition`

  process.env.NODE_ENV !== "production" ? (0, _warning["default"])(expandIconPosition !== 'left' && expandIconPosition !== 'right', 'Collapse', '`expandIconPosition` with `left` or `right` is deprecated. Please use `start` or `end` instead.') : void 0; // Align with logic position

  var mergedExpandIconPosition = React.useMemo(function () {
    if (expandIconPosition === 'left') {
      return 'start';
    }

    return expandIconPosition === 'right' ? 'end' : expandIconPosition;
  }, [expandIconPosition]);

  var renderExpandIcon = function renderExpandIcon() {
    var panelProps = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};
    var expandIcon = props.expandIcon;
    var icon = expandIcon ? expandIcon(panelProps) : /*#__PURE__*/React.createElement(_RightOutlined["default"], {
      rotate: panelProps.isActive ? 90 : undefined
    });
    return (0, _reactNode.cloneElement)(icon, function () {
      return {
        className: (0, _classnames["default"])(icon.props.className, "".concat(prefixCls, "-arrow"))
      };
    });
  };

  var collapseClassName = (0, _classnames["default"])("".concat(prefixCls, "-icon-position-").concat(mergedExpandIconPosition), (_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-borderless"), !bordered), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-rtl"), direction === 'rtl'), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-ghost"), !!ghost), _classNames), className);
  var openMotion = (0, _extends2["default"])((0, _extends2["default"])({}, _motion["default"]), {
    motionAppear: false,
    leavedClassName: "".concat(prefixCls, "-content-hidden")
  });

  var getItems = function getItems() {
    var children = props.children;
    return (0, _toArray["default"])(children).map(function (child, index) {
      var _a;

      if ((_a = child.props) === null || _a === void 0 ? void 0 : _a.disabled) {
        var key = child.key || String(index);
        var _child$props = child.props,
            disabled = _child$props.disabled,
            collapsible = _child$props.collapsible;
        var childProps = (0, _extends2["default"])((0, _extends2["default"])({}, (0, _omit["default"])(child.props, ['disabled'])), {
          key: key,
          collapsible: collapsible !== null && collapsible !== void 0 ? collapsible : disabled ? 'disabled' : undefined
        });
        return (0, _reactNode.cloneElement)(child, childProps);
      }

      return child;
    });
  };

  return /*#__PURE__*/React.createElement(_rcCollapse["default"], (0, _extends2["default"])({
    openMotion: openMotion
  }, props, {
    expandIcon: renderExpandIcon,
    prefixCls: prefixCls,
    className: collapseClassName
  }), getItems());
};

Collapse.Panel = _CollapsePanel["default"];
var _default = Collapse;
exports["default"] = _default;