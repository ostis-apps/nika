const express = require('express');

const app = express();

app.use(express.json());

require('dotenv').config();

const express = require('express');
const path = require('path');
const app = express();
const server = require('http').createServer(app);
const io = require('socket.io')(server);

const DEFAULT_PORT = 8090;
const PORT = process.env.PORT ?? DEFAULT_PORT;

const users = [];

const hasUser = (socketId) => !!users.find((user) => user.socketId === socketId);

io.on('connection', (socket) => {
    console.log('::connected::');

    socket.on('request-start-call', (to) => {
        const toUser = users.find((user) => user.id === to);
        const fromUser = users.find((user) => user.socketId === socket.id);
        if (!toUser || !fromUser) return;
        socket.to(toUser.socketId).emit('request-start-call', fromUser.id);
    });

    socket.on('reset-call', (to) => {
        const toUser = users.find((user) => user.id === to);
        if (!toUser) return;
        socket.to(toUser.socketId).emit('reset-call');
    });

    socket.on('offer', (offer, to) => {
        const toUser = users.find((user) => user.id === to);
        if (!toUser) return;
        socket.to(toUser.socketId).emit('offer', offer);
    });

    socket.on('candidate', (candidate, to) => {
        const toUser = users.find((user) => user.id === to);
        if (!toUser) return;
        socket.to(toUser.socketId).emit('candidate', candidate);
    });

    socket.on('answer', (answer, to) => {
        const toUser = users.find((user) => user.id === to);
        console.log('answer:: ', { to, toUser });
        if (!toUser) return;
        socket.to(toUser.socketId).emit('answer', answer);
    });

    socket.on('resetCall', (to) => {
        const toUser = users.find((user) => user.id === to);
        if (!toUser) return;
        socket.to(toUser.socketId).emit('resetCall');
    });

    socket.on('join', (user) => {
        if (hasUser(socket.id)) return;

        users.push({ ...user, socketId: socket.id });
        console.log('join:: ', users);
        io.emit('users-online', [...users]);
    });

    socket.on('get-users-online', () => {
        io.emit('users-online', [...users]);
    });

    socket.on('disconnecting', () => {
        const targetUserInd = users.findIndex((user) => user.id === socket.id);
        users.splice(targetUserInd, 1);
        console.log('leave:: ', users);
        io.emit('users-online', [...users]);
    });
});

app.use(express.static(path.join(__dirname, '../build')));

app.engine('html', require('ejs').renderFile);
app.set('view engine', 'html');
app.set('views', path.resolve(__dirname, '../build'));
app.use(express.json());
app.get('*', (req, res) => {
    res.render('index');
});

server.listen(PORT, () => console.log(`Listen on port ${PORT}`));