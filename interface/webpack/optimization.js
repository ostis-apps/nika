const { IS_PROD } = require('./env');
const TerserWebpackPlugin = require('terser-webpack-plugin');

const runtimeChunk = IS_PROD ? {} : { runtimeChunk: 'single' };

module.exports = {
    minimize: IS_PROD,
    minimizer: [new TerserWebpackPlugin({ extractComments: false })],
    ...runtimeChunk,
    moduleIds: IS_PROD ? 'deterministic' : 'natural',
};
