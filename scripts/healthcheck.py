import argparse

import json
from websocket import create_connection, _exceptions

SC_SERVER_HOST = "host"
SC_SERVER_PORT = "port"
SC_SERVER_TIMEOUT = "timeout"

SC_SERVER_HOST_DEFAULT = "localhost"
SC_SERVER_PORT_DEFAULT = "8090"
SC_SERVER_TIMEOUT_VALUE = 5  # seconds


def main(args: dict):
    try:
        ws = create_connection(f"ws://{args[SC_SERVER_HOST]}:{args[SC_SERVER_PORT]}", timeout=args[SC_SERVER_TIMEOUT])
    except _exceptions.WebSocketTimeoutException as e:
        print("Connection sc-server timed out")
        exit(1)
    except Exception as e:
        print(e)
        exit(1)

    ws.send(json.dumps({"type": "healthcheck"}))

    try:
        result = ws.recv()
    except _exceptions.WebSocketTimeoutException as e:
        print("Sc-server response timed out")
        exit(1)

    ws.close()

    if json.loads(result) == "OK":
        exit(0)
    else:
        exit(1)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument(
        '--host', type=str, dest=SC_SERVER_HOST, default=SC_SERVER_HOST_DEFAULT, help="Sc-server host")
    parser.add_argument(
        '--port', type=int, dest=SC_SERVER_PORT, default=SC_SERVER_PORT_DEFAULT, help="Sc-server port")
    parser.add_argument(
        '--timeout', '-t', type=int, dest=SC_SERVER_TIMEOUT, default=SC_SERVER_TIMEOUT_VALUE, help="Sc-server timeout")
    args = parser.parse_args()

    main(vars(args))
