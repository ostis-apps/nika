"use strict";

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.getMergedStatus = void 0;
exports.getStatusClassNames = getStatusClassNames;

var _defineProperty2 = _interopRequireDefault(require("@babel/runtime/helpers/defineProperty"));

var _classnames = _interopRequireDefault(require("classnames"));

var _type = require("./type");

var InputStatuses = (0, _type.tuple)('warning', 'error', '');

function getStatusClassNames(prefixCls, status, hasFeedback) {
  var _classNames;

  return (0, _classnames["default"])((_classNames = {}, (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-status-success"), status === 'success'), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-status-warning"), status === 'warning'), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-status-error"), status === 'error'), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-status-validating"), status === 'validating'), (0, _defineProperty2["default"])(_classNames, "".concat(prefixCls, "-has-feedback"), hasFeedback), _classNames));
}

var getMergedStatus = function getMergedStatus(contextStatus, customStatus) {
  return customStatus || contextStatus;
};

exports.getMergedStatus = getMergedStatus;