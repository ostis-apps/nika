"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = exports.Option = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _slicedToArray2 = _interopRequireDefault(require("@babel/runtime/helpers/slicedToArray"));

var _classnames = _interopRequireDefault(require("classnames"));

var _rcMentions = _interopRequireDefault(require("rc-mentions"));

var _ref = require("rc-util/lib/ref");

var React = _interopRequireWildcard(require("react"));

var _configProvider = require("../config-provider");

var _defaultRenderEmpty = _interopRequireDefault(require("../config-provider/defaultRenderEmpty"));

var _context = require("../form/context");

var _spin = _interopRequireDefault(require("../spin"));

var _statusUtils = require("../_util/statusUtils");

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

var Option = _rcMentions["default"].Option;
exports.Option = Option;

function loadingFilterOption() {
  return true;
}

var InternalMentions = function InternalMentions(_a, ref) {
  var _classNames;

  var customizePrefixCls = _a.prefixCls,
      className = _a.className,
      disabled = _a.disabled,
      loading = _a.loading,
      filterOption = _a.filterOption,
      children = _a.children,
      notFoundContent = _a.notFoundContent,
      customStatus = _a.status,
      restProps = __rest(_a, ["prefixCls", "className", "disabled", "loading", "filterOption", "children", "notFoundContent", "status"]);

  var _React$useState = React.useState(false),
      _React$useState2 = (0, _slicedToArray2["default"])(_React$useState, 2),
      focused = _React$useState2[0],
      setFocused = _React$useState2[1];

  var innerRef = React.useRef();
  var mergedRef = (0, _ref.composeRef)(ref, innerRef);

  var _React$useContext = React.useContext(_configProvider.ConfigContext),
      getPrefixCls = _React$useContext.getPrefixCls,
      renderEmpty = _React$useContext.renderEmpty,
      direction = _React$useContext.direction;

  var _React$useContext2 = React.useContext(_context.FormItemInputContext),
      contextStatus = _React$useContext2.status,
      hasFeedback = _React$useContext2.hasFeedback,
      feedbackIcon = _React$useContext2.feedbackIcon;

  var mergedStatus = (0, _statusUtils.getMergedStatus)(contextStatus, customStatus);

  var onFocus = function onFocus() {
    if (restProps.onFocus) {
      restProps.onFocus.apply(restProps, arguments);
    }

    setFocused(true);
  };

  var onBlur = function onBlur() {
    if (restProps.onBlur) {
      restProps.onBlur.apply(restProps, arguments);
    }

    setFocused(false);
  };

  var getNotFoundContent = function getNotFoundContent() {
    if (notFoundContent !== undefined) {
      return notFoundContent;
    }

    return (renderEmpty || _defaultRenderEmpty["default"])('Select');
  };

  var getOptions = function getOptions() {
    if (loading) {
      return /*#__PURE__*/React.createElement(Option, {
        value: "ANTD_SEARCHING",
        disabled: true
      }, /*#__PURE__*/React.createElement(_spin["default"], {
        size: "small"
      }));
    }

    return children;
  };

  var getFilterOption = function getFilterOption() {
    if (loading) {
      return loadingFilterOption;
    }

    return filterOption;
  };

  var prefixCls = getPrefixCls('mentions', customizePrefixCls);
  var mergedClassName = (0, _classnames["default"])((_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-disabled"), disabled), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-focused"), focused), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-rtl"), direction === 'rtl'), _classNames), (0, _statusUtils.getStatusClassNames)(prefixCls, mergedStatus), !hasFeedback && className);
  var mentions = /*#__PURE__*/React.createElement(_rcMentions["default"], (0, _extends2["default"])({
    prefixCls: prefixCls,
    notFoundContent: getNotFoundContent(),
    className: mergedClassName,
    disabled: disabled,
    direction: direction
  }, restProps, {
    filterOption: getFilterOption(),
    onFocus: onFocus,
    onBlur: onBlur,
    ref: mergedRef
  }), getOptions());

  if (hasFeedback) {
    return /*#__PURE__*/React.createElement("div", {
      className: (0, _classnames["default"])("".concat(prefixCls, "-affix-wrapper"), (0, _statusUtils.getStatusClassNames)("".concat(prefixCls, "-affix-wrapper"), mergedStatus, hasFeedback), className)
    }, mentions, /*#__PURE__*/React.createElement("span", {
      className: "".concat(prefixCls, "-suffix")
    }, feedbackIcon));
  }

  return mentions;
};

var Mentions = /*#__PURE__*/React.forwardRef(InternalMentions);

if (process.env.NODE_ENV !== 'production') {
  Mentions.displayName = 'Mentions';
}

Mentions.Option = Option;

Mentions.getMentions = function () {
  var value = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : '';
  var config = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : {};
  var _config$prefix = config.prefix,
      prefix = _config$prefix === void 0 ? '@' : _config$prefix,
      _config$split = config.split,
      split = _config$split === void 0 ? ' ' : _config$split;
  var prefixList = Array.isArray(prefix) ? prefix : [prefix];
  return value.split(split).map(function () {
    var str = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : '';
    var hitPrefix = null;
    prefixList.some(function (prefixStr) {
      var startStr = str.slice(0, prefixStr.length);

      if (startStr === prefixStr) {
        hitPrefix = prefixStr;
        return true;
      }

      return false;
    });

    if (hitPrefix !== null) {
      return {
        prefix: hitPrefix,
        value: str.slice(hitPrefix.length)
      };
    }

    return null;
  }).filter(function (entity) {
    return !!entity && !!entity.value;
  });
};

var _default = Mentions;
exports["default"] = _default;