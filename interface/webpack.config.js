require('dotenv').config();

const { merge } = require('webpack-merge');
const baseConfig = require('./webpack/webpack.base');
const initPresets = require('./webpack/presets');

module.exports = (env) => merge(baseConfig(env), initPresets(env));
