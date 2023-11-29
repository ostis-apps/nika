"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _typeof2 = _interopRequireDefault(require("@babel/runtime/helpers/typeof"));

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _HolderOutlined = _interopRequireDefault(require("@ant-design/icons/HolderOutlined"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcTree = _interopRequireWildcard(require("rc-tree"));

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _motion = _interopRequireDefault(require("../_util/motion"));

var _DirectoryTree = _interopRequireDefault(require("./DirectoryTree"));

var _dropIndicator = _interopRequireDefault(require("./utils/dropIndicator"));

var _iconUtil = _interopRequireDefault(require("./utils/iconUtil"));

var Tree = /*#__PURE__*/React.forwardRef(function (props, ref) {
  var _classNames;

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      direction = _React$useContext.direction,
      virtual = _React$useContext.virtual;

  var customizePrefixCls = props.prefixCls,
      className = props.className,
      showIcon = props.showIcon,
      showLine = props.showLine,
      _switcherIcon = props.switcherIcon,
      blockNode = props.blockNode,
      children = props.children,
      checkable = props.checkable,
      selectable = props.selectable,
      draggable = props.draggable;
  var prefixCls = getPrefixCls('tree', customizePrefixCls);
  var newProps = (0, _extends2["default"])((0, _extends2["default"])({}, props), {
    showLine: Boolean(showLine),
    dropIndicatorRender: _dropIndicator["default"]
  });
  var draggableConfig = React.useMemo(function () {
    if (!draggable) {
      return false;
    }

    var mergedDraggable = {};

    switch ((0, _typeof2["default"])(draggable)) {
      case 'function':
        mergedDraggable.nodeDraggable = draggable;
        break;

      case 'object':
        mergedDraggable = (0, _extends2["default"])({}, draggable);
        break;

      default: // Do nothing

    }

    if (mergedDraggable.icon !== false) {
      mergedDraggable.icon = mergedDraggable.icon || /*#__PURE__*/React.createElement(_HolderOutlined["default"], null);
    }

    return mergedDraggable;
  }, [draggable]);
  return /*#__PURE__*/React.createElement(_rcTree["default"], (0, _extends2["default"])({
    itemHeight: 20,
    ref: ref,
    virtual: virtual
  }, newProps, {
    prefixCls: prefixCls,
    className: (0, _classnames["default"])((_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-icon-hide"), !showIcon), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-block-node"), blockNode), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-unselectable"), !selectable), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-rtl"), direction === 'rtl'), _classNames), className),
    direction: direction,
    checkable: checkable ? /*#__PURE__*/React.createElement("span", {
      className: "".concat(prefixCls, "-checkbox-inner")
    }) : checkable,
    selectable: selectable,
    switcherIcon: function switcherIcon(nodeProps) {
      return (0, _iconUtil["default"])(prefixCls, _switcherIcon, showLine, nodeProps);
    },
    draggable: draggableConfig
  }), children);
});
Tree.TreeNode = _rcTree.TreeNode;
Tree.DirectoryTree = _DirectoryTree["default"];
Tree.defaultProps = {
  checkable: false,
  selectable: true,
  showIcon: false,
  motion: (0, _extends2["default"])((0, _extends2["default"])({}, _motion["default"]), {
    motionAppear: false
  }),
  blockNode: false
};
var _default = Tree;
exports["default"] = _default;