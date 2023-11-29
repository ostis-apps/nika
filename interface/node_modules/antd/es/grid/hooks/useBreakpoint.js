import { useEffect, useRef } from 'react';
import useForceUpdate from '../../_util/hooks/useForceUpdate';
import ResponsiveObserve from '../../_util/responsiveObserve';

function useBreakpoint() {
  var refreshOnChange = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : true;
  var screensRef = useRef({});
  var forceUpdate = useForceUpdate();
  useEffect(function () {
    var token = ResponsiveObserve.subscribe(function (supportScreens) {
      screensRef.current = supportScreens;

      if (refreshOnChange) {
        forceUpdate();
      }
    });
    return function () {
      return ResponsiveObserve.unsubscribe(token);
    };
  }, []);
  return screensRef.current;
}

export default useBreakpoint;