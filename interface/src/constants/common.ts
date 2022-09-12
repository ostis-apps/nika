const defaultSocketUrl = 'ws://localhost:8090';
const defaultScUrl = 'ws://localhost:8090';
const defaultScScWebUrl = 'http://localhost:8000'

export const SC_URL = process.env.SC_URL ?? defaultScUrl;
export const SOCKET_URL = process.env.SOCKET_URL ?? defaultSocketUrl;
export const SC_WEB_URL = process.env.SC_WEB_URL ?? defaultScScWebUrl;
