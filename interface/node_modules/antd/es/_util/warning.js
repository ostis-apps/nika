import rcWarning, { resetWarned } from "rc-util/es/warning";
export { resetWarned };
export function noop() {} // eslint-disable-next-line import/no-mutable-exports

var warning = noop;

if (process.env.NODE_ENV !== 'production') {
  warning = function warning(valid, component, message) {
    rcWarning(valid, "[antd: ".concat(component, "] ").concat(message)); // StrictMode will inject console which will not throw warning in React 17.

    if (process.env.NODE_ENV === 'test') {
      resetWarned();
    }
  };
}

export default warning;