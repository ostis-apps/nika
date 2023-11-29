import warning from '../_util/warning';

var Icon = function Icon() {
  process.env.NODE_ENV !== "production" ? warning(false, 'Icon', 'Empty Icon') : void 0;
  return null;
};

export default Icon;