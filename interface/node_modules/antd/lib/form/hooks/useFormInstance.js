"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = useFormInstance;

var _react = require("react");

var _context = require("../context");

function useFormInstance() {
  var _useContext = (0, _react.useContext)(_context.FormContext),
      form = _useContext.form;

  return form;
}