import _defineProperty from "@babel/runtime/helpers/esm/defineProperty";
import classNames from 'classnames';
import { tuple } from './type';
var InputStatuses = tuple('warning', 'error', '');
export function getStatusClassNames(prefixCls, status, hasFeedback) {
  var _classNames;

  return classNames((_classNames = {}, _defineProperty(_classNames, "".concat(prefixCls, "-status-success"), status === 'success'), _defineProperty(_classNames, "".concat(prefixCls, "-status-warning"), status === 'warning'), _defineProperty(_classNames, "".concat(prefixCls, "-status-error"), status === 'error'), _defineProperty(_classNames, "".concat(prefixCls, "-status-validating"), status === 'validating'), _defineProperty(_classNames, "".concat(prefixCls, "-has-feedback"), hasFeedback), _classNames));
}
export var getMergedStatus = function getMergedStatus(contextStatus, customStatus) {
  return customStatus || contextStatus;
};