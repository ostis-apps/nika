const Dotenv = require('dotenv-webpack');
const { relativeRoot } = require('../utils');

module.exports = (path) =>
    new Dotenv({
        path: path || relativeRoot('.env'),
    });
