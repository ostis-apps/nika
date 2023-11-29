import _extends from "@babel/runtime/helpers/esm/extends";
import * as React from 'react';
import Select from '../select';

var MiniSelect = function MiniSelect(props) {
  return /*#__PURE__*/React.createElement(Select, _extends({}, props, {
    size: "small"
  }));
};

var MiddleSelect = function MiddleSelect(props) {
  return /*#__PURE__*/React.createElement(Select, _extends({}, props, {
    size: "middle"
  }));
};

MiniSelect.Option = Select.Option;
MiddleSelect.Option = Select.Option;
export { MiniSelect, MiddleSelect };