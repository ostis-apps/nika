const { IS_DEV } = require('../env');
const htmlWebpackPlugin = require('./htmlWebpackPlugin');
const progressPlugin = require('./progressPlugin');
const definePlugin = require('./definePlugin');
const reactRefreshWebpackPlugin = require('./reactRefreshWebpackPlugin');
const forkTsCheckerWebpackPlugin = require('./forkTsCheckerWebpackPlugin');
const cleanWebpackPlugin = require('./cleanWebpackPlugin');
const dotEnvPlugin = require('./dotEnvPlugin');

module.exports = (env) =>
    [
        htmlWebpackPlugin,
        progressPlugin,
        definePlugin,
        forkTsCheckerWebpackPlugin,
        cleanWebpackPlugin,
        dotEnvPlugin(env.envFilePath),
        IS_DEV && reactRefreshWebpackPlugin,
    ].filter(Boolean);
