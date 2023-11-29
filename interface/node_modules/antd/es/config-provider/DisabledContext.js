import * as React from 'react';
var DisabledContext = /*#__PURE__*/React.createContext(false);
export var DisabledContextProvider = function DisabledContextProvider(_ref) {
  var children = _ref.children,
      disabled = _ref.disabled;
  var originDisabled = React.useContext(DisabledContext);
  return /*#__PURE__*/React.createElement(DisabledContext.Provider, {
    value: disabled || originDisabled
  }, children);
};
export default DisabledContext;