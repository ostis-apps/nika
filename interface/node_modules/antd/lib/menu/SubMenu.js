"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcMenu = require("rc-menu");

var _omit = _interopRequireDefault(require("rc-util/lib/omit"));

var React = _interopRequireWildcard(require("react"));

var _reactNode = require("../_util/reactNode");

var _MenuContext = _interopRequireDefault(require("./MenuContext"));

function SubMenu(props) {
  var _a;

  var popupClassName = props.popupClassName,
      icon = props.icon,
      title = props.title,
      theme = props.theme;
  var context = React.useContext(_MenuContext["default"]);
  var prefixCls = context.prefixCls,
      inlineCollapsed = context.inlineCollapsed,
      antdMenuTheme = context.antdMenuTheme;
  var parentPath = (0, _rcMenu.useFullPath)();
  var titleNode;

  if (!icon) {
    titleNode = inlineCollapsed && !parentPath.length && title && typeof title === 'string' ? /*#__PURE__*/React.createElement("div", {
      className: "".concat(prefixCls, "-inline-collapsed-noicon")
    }, title.charAt(0)) : /*#__PURE__*/React.createElement("span", {
      className: "".concat(prefixCls, "-title-content")
    }, title);
  } else {
    // inline-collapsed.md demo 依赖 span 来隐藏文字,有 icon 属性，则内部包裹一个 span
    // ref: https://github.com/ant-design/ant-design/pull/23456
    var titleIsSpan = (0, _reactNode.isValidElement)(title) && title.type === 'span';
    titleNode = /*#__PURE__*/React.createElement(React.Fragment, null, (0, _reactNode.cloneElement)(icon, {
      className: (0, _classnames["default"])((0, _reactNode.isValidElement)(icon) ? (_a = icon.props) === null || _a === void 0 ? void 0 : _a.className : '', "".concat(prefixCls, "-item-icon"))
    }), titleIsSpan ? title : /*#__PURE__*/React.createElement("span", {
      className: "".concat(prefixCls, "-title-content")
    }, title));
  }

  var contextValue = React.useMemo(function () {
    return (0, _extends2["default"])((0, _extends2["default"])({}, context), {
      firstLevel: false
    });
  }, [context]);
  return /*#__PURE__*/React.createElement(_MenuContext["default"].Provider, {
    value: contextValue
  }, /*#__PURE__*/React.createElement(_rcMenu.SubMenu, (0, _extends2["default"])({}, (0, _omit["default"])(props, ['icon']), {
    title: titleNode,
    popupClassName: (0, _classnames["default"])(prefixCls, "".concat(prefixCls, "-").concat(theme || antdMenuTheme), popupClassName)
  })));
}

var _default = SubMenu;
exports["default"] = _default;