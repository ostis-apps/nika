# nika-ui
NIKA User Interface

## Run 

### Install dependencies
```sh
yarn
```

### Run for development
```sh
# Start webpack dev server
yarn start

# Start local websocket server
yarn serve
```

### Run for production:
```sh
# create build folder
yarn build
# serve content from created build folder
yarn serve
```

If you have no need of overriding default api urls (described below) you can use one command
```sh
yarn build:serve
```

### Change urls of sc-server

If one would like to pass another urls it could be done by specifying `.env` file. By default the root one used (relative to `package.json`). This behavior may be overridden by passing `--env envFilePath='./your-path'` to `yarn start` command.

Example for development

```sh
yarn start --env envFilePath='./your-path'
```

Example for production

```sh
yarn build --env envFilePath='./your-path'
yarn serve
```
