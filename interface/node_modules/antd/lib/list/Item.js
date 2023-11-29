"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = exports.Meta = void 0;

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _classnames = _interopRequireDefault(require("classnames"));

var _react = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _grid = require("../grid");

var _reactNode = require("../_util/reactNode");

var _index = require("./index");

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

var Meta = function Meta(_a) {
  var customizePrefixCls = _a.prefixCls,
      className = _a.className,
      avatar = _a.avatar,
      title = _a.title,
      description = _a.description,
      others = __rest(_a, ["prefixCls", "className", "avatar", "title", "description"]);

  var _useContext = (0, _react.useContext)(_configProvider.ConfigContext),
      getPrefixCls = _useContext.getPrefixCls;

  var prefixCls = getPrefixCls('list', customizePrefixCls);
  var classString = (0, _classnames["default"])("".concat(prefixCls, "-item-meta"), className);

  var content = /*#__PURE__*/_react["default"].createElement("div", {
    className: "".concat(prefixCls, "-item-meta-content")
  }, title && /*#__PURE__*/_react["default"].createElement("h4", {
    className: "".concat(prefixCls, "-item-meta-title")
  }, title), description && /*#__PURE__*/_react["default"].createElement("div", {
    className: "".concat(prefixCls, "-item-meta-description")
  }, description));

  return /*#__PURE__*/_react["default"].createElement("div", (0, _extends2["default"])({}, others, {
    className: classString
  }), avatar && /*#__PURE__*/_react["default"].createElement("div", {
    className: "".concat(prefixCls, "-item-meta-avatar")
  }, avatar), (title || description) && content);
};

exports.Meta = Meta;

var InternalItem = function InternalItem(_a, ref) {
  var customizePrefixCls = _a.prefixCls,
      children = _a.children,
      actions = _a.actions,
      extra = _a.extra,
      className = _a.className,
      colStyle = _a.colStyle,
      others = __rest(_a, ["prefixCls", "children", "actions", "extra", "className", "colStyle"]);

  var _useContext2 = (0, _react.useContext)(_index.ListContext),
      grid = _useContext2.grid,
      itemLayout = _useContext2.itemLayout;

  var _useContext3 = (0, _react.useContext)(_configProvider.ConfigContext),
      getPrefixCls = _useContext3.getPrefixCls;

  var isItemContainsTextNodeAndNotSingular = function isItemContainsTextNodeAndNotSingular() {
    var result;

    _react.Children.forEach(children, function (element) {
      if (typeof element === 'string') {
        result = true;
      }
    });

    return result && _react.Children.count(children) > 1;
  };

  var isFlexMode = function isFlexMode() {
    if (itemLayout === 'vertical') {
      return !!extra;
    }

    return !isItemContainsTextNodeAndNotSingular();
  };

  var prefixCls = getPrefixCls('list', customizePrefixCls);

  var actionsContent = actions && actions.length > 0 && /*#__PURE__*/_react["default"].createElement("ul", {
    className: "".concat(prefixCls, "-item-action"),
    key: "actions"
  }, actions.map(function (action, i) {
    return (
      /*#__PURE__*/
      // eslint-disable-next-line react/no-array-index-key
      _react["default"].createElement("li", {
        key: "".concat(prefixCls, "-item-action-").concat(i)
      }, action, i !== actions.length - 1 && /*#__PURE__*/_react["default"].createElement("em", {
        className: "".concat(prefixCls, "-item-action-split")
      }))
    );
  }));

  var Element = grid ? 'div' : 'li';

  var itemChildren = /*#__PURE__*/_react["default"].createElement(Element, (0, _extends2["default"])({}, others, !grid ? {
    ref: ref
  } : {}, {
    className: (0, _classnames["default"])("".concat(prefixCls, "-item"), (0, _defineProperty2["default"])({}, "".concat(prefixCls, "-item-no-flex"), !isFlexMode()), className)
  }), itemLayout === 'vertical' && extra ? [/*#__PURE__*/_react["default"].createElement("div", {
    className: "".concat(prefixCls, "-item-main"),
    key: "content"
  }, children, actionsContent), /*#__PURE__*/_react["default"].createElement("div", {
    className: "".concat(prefixCls, "-item-extra"),
    key: "extra"
  }, extra)] : [children, actionsContent, (0, _reactNode.cloneElement)(extra, {
    key: 'extra'
  })]);

  return grid ? /*#__PURE__*/_react["default"].createElement(_grid.Col, {
    ref: ref,
    flex: 1,
    style: colStyle
  }, itemChildren) : itemChildren;
};

var Item = /*#__PURE__*/(0, _react.forwardRef)(InternalItem);
Item.Meta = Meta;
var _default = Item;
exports["default"] = _default;