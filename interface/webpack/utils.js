const path = require('path');

const relativeRoot = (relativePath) => path.resolve(process.cwd(), relativePath);

module.exports = { relativeRoot };
