import warning from '../_util/warning';
export function validProgress(progress) {
  if (!progress || progress < 0) {
    return 0;
  }

  if (progress > 100) {
    return 100;
  }

  return progress;
}
export function getSuccessPercent(_ref) {
  var success = _ref.success,
      successPercent = _ref.successPercent;
  var percent = successPercent;
  /** @deprecated Use `percent` instead */

  if (success && 'progress' in success) {
    process.env.NODE_ENV !== "production" ? warning(false, 'Progress', '`success.progress` is deprecated. Please use `success.percent` instead.') : void 0;
    percent = success.progress;
  }

  if (success && 'percent' in success) {
    percent = success.percent;
  }

  return percent;
}