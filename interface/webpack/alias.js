const { relativeRoot } = require('./utils');

module.exports = {
    '@model': relativeRoot('src/model'),
    '@constants': relativeRoot('src/constants'),
    '@pages': relativeRoot('src/pages'),
    '@hooks': relativeRoot('src/hooks'),
    '@components': relativeRoot('src/components'),
    '@api': relativeRoot('src/api'),
    '@store': relativeRoot('src/store'),
    '@assets': relativeRoot('src/assets'),
    '@utils': relativeRoot('src/utils'),
    '@agents': relativeRoot('src/api/sc/agents/'),
};
