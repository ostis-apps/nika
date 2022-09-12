import { SOCKET_URL } from '@constants';
import { io } from 'socket.io-client';

const options = {
    'force new connection': true,
    reconnectionAttempts: 500,
    // reconnectionAttempts: 'Infinity', // avoid having user reconnect manually in order to prevent dead clients after a server restart
    timeout: 10000, // before connect_error and connect_timeout are emitted.
    transports: ['websocket'],
};

export const socket = io(SOCKET_URL, options);
