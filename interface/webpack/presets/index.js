const { merge } = require('webpack-merge');

module.exports = ({ presets, ...restEnv }) => {
  const splitedPresets = presets ? presets.split(',') : [];
  const mergedPresets = splitedPresets.map((presetName) => {
    const presetConfig = require(`./webpack.${presetName}`);
    return typeof presetConfig === 'function'
      ? presetConfig({ ...restEnv })
      : presetConfig;
  });
  return merge({}, ...mergedPresets);
};
