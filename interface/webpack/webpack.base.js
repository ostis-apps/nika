const { NODE_ENV, IS_DEV } = require('./env');
const getPlugins = require('./plugins');
const devServer = require('./devServer');
const rules = require('./rules');
const optimization = require('./optimization');
const alias = require('./alias');
const { relativeRoot } = require('./utils');

module.exports = (env) => ({
    mode: NODE_ENV,
    resolve: {
        alias,
        extensions: ['.js', '.ts', '.tsx', '.css'],
    },
    entry: relativeRoot('src/index.tsx'),
    output: {
        path: relativeRoot('build'),
        filename: IS_DEV ? '[name].js' : '[name].[contenthash].js',
        publicPath: 'auto',
    },
    optimization,
    devServer,
    devtool: IS_DEV ? 'inline-source-map' : 'hidden-nosources-source-map',
    plugins: getPlugins(env),
    module: {
        rules,
    },
});
