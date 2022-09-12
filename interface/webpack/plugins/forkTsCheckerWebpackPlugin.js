const ForkTsCheckerWebpackPlugin = require('fork-ts-checker-webpack-plugin');

module.exports = new ForkTsCheckerWebpackPlugin({
    typescript: {
        diagnosticOptions: {
            semantic: true,
            syntactic: true,
        },
        mode: 'write-references',
    },
});
