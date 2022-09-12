const express = require('express');
const http = require('http');
const WebSocket = require('ws');

const app = express();
const route = new express.Router();

const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

app.use('/ws_json', route);

wss.on('connection', (ws) => {
    console.log('::connection::');

    ws.on('message', (message) => {
        console.log('message:: ', String(message));
        ws.send(String(message));
    });
});

server.listen(8090, () => {
    console.log(`Server started on port ${server.address().port} :)`);
});
