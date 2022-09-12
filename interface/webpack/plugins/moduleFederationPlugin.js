const path = require('path');
const ModuleFederationPlugin = require('webpack/lib/container/ModuleFederationPlugin');
const deps = require('../../package.json').dependencies;

const config = {
  name: 'NAME',
  filename: 'remoteEntry.js',
  exposes: {
    './SomeLib': path.resolve(process.cwd(), 'src/init.tsx'),
  },
  shared: {
    react: {
      singleton: true,
      requiredVersion: deps.react,
    },
    'react-dom': {
      singleton: true,
      requiredVersion: deps['react-dom'],
    },
  },
};

module.exports = new ModuleFederationPlugin(config);
