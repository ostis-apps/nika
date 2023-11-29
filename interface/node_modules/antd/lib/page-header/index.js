"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _slicedToArray2 = _interopRequireDefault(require("@babel/runtime/helpers/slicedToArray"));

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _ArrowLeftOutlined = _interopRequireDefault(require("@ant-design/icons/ArrowLeftOutlined"));

var _ArrowRightOutlined = _interopRequireDefault(require("@ant-design/icons/ArrowRightOutlined"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcResizeObserver = _interopRequireDefault(require("rc-resize-observer"));

var _useState3 = _interopRequireDefault(require("rc-util/lib/hooks/useState"));

var React = _interopRequireWildcard(require("react"));

var _avatar = _interopRequireDefault(require("../avatar"));

var _breadcrumb = _interopRequireDefault(require("../breadcrumb"));

var _configProvider = require("../config-provider");

var _LocaleReceiver = _interopRequireDefault(require("../locale-provider/LocaleReceiver"));

var _space = _interopRequireDefault(require("../space"));

var _transButton = _interopRequireDefault(require("../_util/transButton"));

var renderBack = function renderBack(prefixCls, backIcon, onBack) {
  if (!backIcon || !onBack) {
    return null;
  }

  return /*#__PURE__*/React.createElement(_LocaleReceiver["default"], {
    componentName: "PageHeader"
  }, function (_ref) {
    var back = _ref.back;
    return /*#__PURE__*/React.createElement("div", {
      className: "".concat(prefixCls, "-back")
    }, /*#__PURE__*/React.createElement(_transButton["default"], {
      onClick: function onClick(e) {
        onBack === null || onBack === void 0 ? void 0 : onBack(e);
      },
      className: "".concat(prefixCls, "-back-button"),
      "aria-label": back
    }, backIcon));
  });
};

var renderBreadcrumb = function renderBreadcrumb(breadcrumb) {
  return /*#__PURE__*/React.createElement(_breadcrumb["default"], (0, _extends2["default"])({}, breadcrumb));
};

var getBackIcon = function getBackIcon(props) {
  var direction = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 'ltr';

  if (props.backIcon !== undefined) {
    return props.backIcon;
  }

  return direction === 'rtl' ? /*#__PURE__*/React.createElement(_ArrowRightOutlined["default"], null) : /*#__PURE__*/React.createElement(_ArrowLeftOutlined["default"], null);
};

var renderTitle = function renderTitle(prefixCls, props) {
  var direction = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : 'ltr';
  var title = props.title,
      avatar = props.avatar,
      subTitle = props.subTitle,
      tags = props.tags,
      extra = props.extra,
      onBack = props.onBack;
  var headingPrefixCls = "".concat(prefixCls, "-heading");
  var hasHeading = title || subTitle || tags || extra; // If there is nothing, return a null

  if (!hasHeading) {
    return null;
  }

  var backIcon = getBackIcon(props, direction);
  var backIconDom = renderBack(prefixCls, backIcon, onBack);
  var hasTitle = backIconDom || avatar || hasHeading;
  return /*#__PURE__*/React.createElement("div", {
    className: headingPrefixCls
  }, hasTitle && /*#__PURE__*/React.createElement("div", {
    className: "".concat(headingPrefixCls, "-left")
  }, backIconDom, avatar && /*#__PURE__*/React.createElement(_avatar["default"], (0, _extends2["default"])({}, avatar)), title && /*#__PURE__*/React.createElement("span", {
    className: "".concat(headingPrefixCls, "-title"),
    title: typeof title === 'string' ? title : undefined
  }, title), subTitle && /*#__PURE__*/React.createElement("span", {
    className: "".concat(headingPrefixCls, "-sub-title"),
    title: typeof subTitle === 'string' ? subTitle : undefined
  }, subTitle), tags && /*#__PURE__*/React.createElement("span", {
    className: "".concat(headingPrefixCls, "-tags")
  }, tags)), extra && /*#__PURE__*/React.createElement("span", {
    className: "".concat(headingPrefixCls, "-extra")
  }, /*#__PURE__*/React.createElement(_space["default"], null, extra)));
};

var renderFooter = function renderFooter(prefixCls, footer) {
  if (footer) {
    return /*#__PURE__*/React.createElement("div", {
      className: "".concat(prefixCls, "-footer")
    }, footer);
  }

  return null;
};

var renderChildren = function renderChildren(prefixCls, children) {
  return /*#__PURE__*/React.createElement("div", {
    className: "".concat(prefixCls, "-content")
  }, children);
};

var PageHeader = function PageHeader(props) {
  var _useState = (0, _useState3["default"])(false),
      _useState2 = (0, _slicedToArray2["default"])(_useState, 2),
      compact = _useState2[0],
      updateCompact = _useState2[1];

  var onResize = function onResize(_ref2) {
    var width = _ref2.width;
    updateCompact(width < 768, true);
  };

  return /*#__PURE__*/React.createElement(_configProvider.ConfigConsumer, null, function (_ref3) {
    var _classNames;

    var getPrefixCls = _ref3.getPrefixCls,
        pageHeader = _ref3.pageHeader,
        direction = _ref3.direction;

    var _a;

    var customizePrefixCls = props.prefixCls,
        style = props.style,
        footer = props.footer,
        children = props.children,
        breadcrumb = props.breadcrumb,
        breadcrumbRender = props.breadcrumbRender,
        customizeClassName = props.className;
    var ghost = true; // Use `ghost` from `props` or from `ConfigProvider` instead.

    if ('ghost' in props) {
      ghost = props.ghost;
    } else if (pageHeader && 'ghost' in pageHeader) {
      ghost = pageHeader.ghost;
    }

    var prefixCls = getPrefixCls('page-header', customizePrefixCls);

    var getDefaultBreadcrumbDom = function getDefaultBreadcrumbDom() {
      if (breadcrumb === null || breadcrumb === void 0 ? void 0 : breadcrumb.routes) {
        return renderBreadcrumb(breadcrumb);
      }

      return null;
    };

    var defaultBreadcrumbDom = getDefaultBreadcrumbDom();
    var isBreadcrumbComponent = breadcrumb && 'props' in breadcrumb; // support breadcrumbRender function

    var breadcrumbRenderDomFromProps = (_a = breadcrumbRender === null || breadcrumbRender === void 0 ? void 0 : breadcrumbRender(props, defaultBreadcrumbDom)) !== null && _a !== void 0 ? _a : defaultBreadcrumbDom;
    var breadcrumbDom = isBreadcrumbComponent ? breadcrumb : breadcrumbRenderDomFromProps;
    var className = (0, _classnames["default"])(prefixCls, customizeClassName, (_classNames = {
      'has-breadcrumb': !!breadcrumbDom,
      'has-footer': !!footer
    }, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-ghost"), ghost), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-rtl"), direction === 'rtl'), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-compact"), compact), _classNames));
    return /*#__PURE__*/React.createElement(_rcResizeObserver["default"], {
      onResize: onResize
    }, /*#__PURE__*/React.createElement("div", {
      className: className,
      style: style
    }, breadcrumbDom, renderTitle(prefixCls, props, direction), children && renderChildren(prefixCls, children), renderFooter(prefixCls, footer)));
  });
};

var _default = PageHeader;
exports["default"] = _default;