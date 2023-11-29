import _extends from "@babel/runtime/helpers/esm/extends";
import { FormProvider as RcFormProvider } from 'rc-field-form';
import omit from "rc-util/es/omit";
import * as React from 'react';
import { useContext, useMemo } from 'react';
export var FormContext = /*#__PURE__*/React.createContext({
  labelAlign: 'right',
  vertical: false,
  itemRef: function itemRef() {}
});
export var NoStyleItemContext = /*#__PURE__*/React.createContext(null);
export var FormProvider = function FormProvider(props) {
  var providerProps = omit(props, ['prefixCls']);
  return /*#__PURE__*/React.createElement(RcFormProvider, _extends({}, providerProps));
};
export var FormItemPrefixContext = /*#__PURE__*/React.createContext({
  prefixCls: ''
});
export var FormItemInputContext = /*#__PURE__*/React.createContext({});
export var NoFormStyle = function NoFormStyle(_ref) {
  var children = _ref.children,
      status = _ref.status,
      override = _ref.override;
  var formItemInputContext = useContext(FormItemInputContext);
  var newFormItemInputContext = useMemo(function () {
    var newContext = _extends({}, formItemInputContext);

    if (override) {
      delete newContext.isFormItemInput;
    }

    if (status) {
      delete newContext.status;
      delete newContext.hasFeedback;
      delete newContext.feedbackIcon;
    }

    return newContext;
  }, [status, override, formItemInputContext]);
  return /*#__PURE__*/React.createElement(FormItemInputContext.Provider, {
    value: newFormItemInputContext
  }, children);
};