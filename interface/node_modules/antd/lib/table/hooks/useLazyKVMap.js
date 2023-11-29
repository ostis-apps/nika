"use strict";

var _interopRequireWildcard = require("@babel/runtime/helpers/interopRequireWildcard")["default"];

var _interopRequireDefault = require("@babel/runtime/helpers/interopRequireDefault")["default"];

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = useLazyKVMap;

var _typeof2 = _interopRequireDefault(require("@babel/runtime/helpers/typeof"));

var React = _interopRequireWildcard(require("react"));

function useLazyKVMap(data, childrenColumnName, getRowKey) {
  var mapCacheRef = React.useRef({});

  function getRecordByKey(key) {
    if (!mapCacheRef.current || mapCacheRef.current.data !== data || mapCacheRef.current.childrenColumnName !== childrenColumnName || mapCacheRef.current.getRowKey !== getRowKey) {
      var kvMap = new Map();
      /* eslint-disable no-inner-declarations */

      function dig(records) {
        records.forEach(function (record, index) {
          var rowKey = getRowKey(record, index);
          kvMap.set(rowKey, record);

          if (record && (0, _typeof2["default"])(record) === 'object' && childrenColumnName in record) {
            dig(record[childrenColumnName] || []);
          }
        });
      }
      /* eslint-enable */


      dig(data);
      mapCacheRef.current = {
        data: data,
        childrenColumnName: childrenColumnName,
        kvMap: kvMap,
        getRowKey: getRowKey
      };
    }

    return mapCacheRef.current.kvMap.get(key);
  }

  return [getRecordByKey];
}