const HtmlWebpackPlugin = require('html-webpack-plugin');
const { relativeRoot } = require('../utils');
const config = {
    template: relativeRoot('public/index.html'),
    filename: 'index.html',
};
module.exports = new HtmlWebpackPlugin(config);
