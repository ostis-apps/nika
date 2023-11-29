"use strict";

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _react = require("react");

var _useForceUpdate = _interopRequireDefault(require("../../_util/hooks/useForceUpdate"));

var _responsiveObserve = _interopRequireDefault(require("../../_util/responsiveObserve"));

function useBreakpoint() {
  var refreshOnChange = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : true;
  var screensRef = (0, _react.useRef)({});
  var forceUpdate = (0, _useForceUpdate["default"])();
  (0, _react.useEffect)(function () {
    var token = _responsiveObserve["default"].subscribe(function (supportScreens) {
      screensRef.current = supportScreens;

      if (refreshOnChange) {
        forceUpdate();
      }
    });

    return function () {
      return _responsiveObserve["default"].unsubscribe(token);
    };
  }, []);
  return screensRef.current;
}

var _default = useBreakpoint;
exports["default"] = _default;