"use strict";

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _extends2 = _interopRequireDefault(require("@babel/runtime/helpers/extends"));

var _by_BY = _interopRequireDefault(require("rc-picker/lib/locale/by_BY"));

var _by_BY2 = _interopRequireDefault(require("../../time-picker/locale/by_BY"));

var locale = {
  lang: (0, _extends2["default"])({
    placeholder: 'Выберыце дату',
    yearPlaceholder: 'Выберыце год',
    quarterPlaceholder: 'Выберыце квартал',
    monthPlaceholder: 'Выберыце месяц',
    weekPlaceholder: 'Выберыце тыдзень',
    rangePlaceholder: ['Дата пачатку', 'Дата заканчэння'],
    rangeYearPlaceholder: ['Год пачатку', 'Год заканчэння'],
    rangeQuarterPlaceholder: ['Квартал пачатку', 'Квартал заканчэння'],
    rangeMonthPlaceholder: ['Месяц пачатку', 'Месяц заканчэння'],
    rangeWeekPlaceholder: ['Тыдзень пачаку', 'Тыдзень заканчэння']
  }, _by_BY["default"]),
  timePickerLocale: (0, _extends2["default"])({}, _by_BY2["default"])
};
var _default = locale;
exports["default"] = _default;