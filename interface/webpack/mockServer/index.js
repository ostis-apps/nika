const { Router } = require('express');
const { HttpCode } = require('./constants');
const chalk = require('chalk');

const route = Router();

module.exports = (app) => {
  app.use('/mock', route);

  console.log(chalk.green('\n::MOCK SERVER STARTED::\n\n'));

  route.get('/some/url', (req, res) => {
    return res
      .status(HttpCode.INTERNAL_SERVER_ERROR)
      .json('Ошибка получения списка');
  });
};
