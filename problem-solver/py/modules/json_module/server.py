import argparse
import logging.config
import os

from pathlib import Path
from sc_kpm import ScServer
from json_module import JSONModule
# from sc_kpm import KPM_LOGGER_NAME

KPM_LOGGER_NAME = "test_logger"

SC_SERVER_HOST = "host"
SC_SERVER_PORT = "port"

SC_SERVER_HOST_DEFAULT = "localhost"
SC_SERVER_PORT_DEFAULT = "8090"

logging.config.dictConfig(
    dict(
        version=1,
        disable_existing_loggers=False,
        formatters={
            "common_formatter": {
                "format": "%(asctime)s | %(levelname)s | %(name)s | %(message)s",
                "datefmt": "[%d-%b-%y %H:%M:%S]",
            }
        },
        handlers={
            "stream_handler": {
                "class": "logging.StreamHandler",
                "level": logging.INFO,
                "formatter": "common_formatter",
            },
            "file_handler": {
                "class": "logging.FileHandler",
                "level": logging.DEBUG,
                "filename": Path(__file__).resolve().parent.joinpath("py_sc_kpm.log"),
                "formatter": "common_formatter",
            },
        },
        loggers={
            KPM_LOGGER_NAME: {
                "handlers": ["stream_handler", "file_handler"],
                "level": logging.DEBUG,
            }
        },
    )
)

def main(args_: dict):
    server = ScServer(f"ws://{args_[SC_SERVER_HOST]}:{args_[SC_SERVER_PORT]}")

    with server.connect():
        modules = [
            JSONModule(),
        ]
        server.add_modules(*modules)

        with server.register_modules():
            # server.wait_for_sigint()
            server.remove_modules(*modules)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument(
        '--host', type=str, dest=SC_SERVER_HOST, default=SC_SERVER_HOST_DEFAULT, help="Sc-server host")
    parser.add_argument(
        '--port', type=int, dest=SC_SERVER_PORT, default=SC_SERVER_PORT_DEFAULT, help="Sc-server port")
    args = parser.parse_args()

    # os.system("python main.py")

    main(vars(args))
