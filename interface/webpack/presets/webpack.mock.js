const mock = require('../mockServer');

module.exports = {
  devServer: {
    onBeforeSetupMiddleware(devServer) {
      mock(devServer.app);
    },
  },
};
