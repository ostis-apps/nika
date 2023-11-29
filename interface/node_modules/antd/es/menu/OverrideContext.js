import _extends from "@babel/runtime/helpers/esm/extends";

var __rest = this && this.__rest || function (s, e) {
  var t = {};

  for (var p in s) {
    if (Object.prototype.hasOwnProperty.call(s, p) && e.indexOf(p) < 0) t[p] = s[p];
  }

  if (s != null && typeof Object.getOwnPropertySymbols === "function") for (var i = 0, p = Object.getOwnPropertySymbols(s); i < p.length; i++) {
    if (e.indexOf(p[i]) < 0 && Object.prototype.propertyIsEnumerable.call(s, p[i])) t[p[i]] = s[p[i]];
  }
  return t;
};

import * as React from 'react';
/** @private Internal Usage. Only used for Dropdown component. Do not use this in your production. */

var OverrideContext = /*#__PURE__*/React.createContext(null);
/** @private Internal Usage. Only used for Dropdown component. Do not use this in your production. */

export var OverrideProvider = function OverrideProvider(_a) {
  var children = _a.children,
      restProps = __rest(_a, ["children"]);

  var override = React.useContext(OverrideContext);
  var context = React.useMemo(function () {
    return _extends(_extends({}, override), restProps);
  }, [override, restProps.prefixCls, // restProps.expandIcon, Not mark as deps since this is a ReactNode
  restProps.mode, restProps.selectable // restProps.validator, Not mark as deps since this is a function
  ]);
  return /*#__PURE__*/React.createElement(OverrideContext.Provider, {
    value: context
  }, children);
};
export default OverrideContext;