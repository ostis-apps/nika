"use strict";

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _confirm = _interopRequireWildcard(require("./confirm"));

var _destroyFns = _interopRequireDefault(require("./destroyFns"));

var _Modal = _interopRequireDefault(require("./Modal"));

var _useModal = _interopRequireDefault(require("./useModal"));

function modalWarn(props) {
  return (0, _confirm["default"])((0, _confirm.withWarn)(props));
}

var Modal = _Modal["default"];
Modal.useModal = _useModal["default"];

Modal.info = function infoFn(props) {
  return (0, _confirm["default"])((0, _confirm.withInfo)(props));
};

Modal.success = function successFn(props) {
  return (0, _confirm["default"])((0, _confirm.withSuccess)(props));
};

Modal.error = function errorFn(props) {
  return (0, _confirm["default"])((0, _confirm.withError)(props));
};

Modal.warning = modalWarn;
Modal.warn = modalWarn;

Modal.confirm = function confirmFn(props) {
  return (0, _confirm["default"])((0, _confirm.withConfirm)(props));
};

Modal.destroyAll = function destroyAllFn() {
  while (_destroyFns["default"].length) {
    var close = _destroyFns["default"].pop();

    if (close) {
      close();
    }
  }
};

Modal.config = _confirm.modalGlobalConfig;
var _default = Modal;
exports["default"] = _default;