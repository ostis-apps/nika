const { DefinePlugin } = require('webpack');
const { IS_PROD, IS_DEV } = require('../env');

module.exports = new DefinePlugin({
    IS_PROD,
    IS_DEV,
});
