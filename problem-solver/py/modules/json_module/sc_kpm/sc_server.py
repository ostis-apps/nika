"""
This source file is part of an OSTIS project. For the latest info, see https:#github.com/ostis-ai
Distributed under the MIT License
(See an accompanying file LICENSE or a copy at https:#opensource.org/licenses/MIT)
"""

from __future__ import annotations

import signal
from abc import ABC, abstractmethod
from logging import Logger, getLogger
from typing import Callable

from sc_client import client

from sc_kpm.identifiers import _IdentifiersResolver
from sc_kpm.sc_module import ScModuleAbstract


class ScServerAbstract(ABC):
    """ScServer connects to server and stores"""

    @abstractmethod
    def connect(self) -> _Finisher:
        """Connect to server"""

    @abstractmethod
    def disconnect(self) -> None:
        """Disconnect from the server"""

    @abstractmethod
    def add_modules(self, *modules: ScModuleAbstract) -> None:
        """Add modules to the server and register them if server is registered"""

    @abstractmethod
    def remove_modules(self, *modules: ScModuleAbstract) -> None:
        """Remove modules from the server and unregister them if server is registered"""

    @abstractmethod
    def clear_modules(self) -> None:
        """Remove all modules from the server and unregister them if server is registered"""

    def register_modules(self) -> _Finisher:
        """Register all modules in the server"""

    def unregister_modules(self) -> None:
        """Unregister all modules from the server"""

    def start(self) -> _Finisher:
        """Connect and register modules"""

    def stop(self) -> None:
        """Disconnect and unregister modules"""


class ScServer(ScServerAbstract):
    def __init__(self, sc_server_url: str) -> None:
        self._url: str = sc_server_url
        self._modules: set[ScModuleAbstract] = set()
        self.is_registered = False
        self.logger = getLogger(f"{self.__module__}.{self.__class__.__name__}")

    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({', '.join(map(repr, self._modules))})"

    def connect(self) -> _Finisher:
        client.connect(self._url)
        self.logger.info("Connected by url: %s", repr(self._url))
        _IdentifiersResolver.resolve()
        return _Finisher(self.disconnect, self.logger)

    def disconnect(self) -> None:
        client.disconnect()
        self.logger.info("Disconnected from url: %s", repr(self._url))

    def add_modules(self, *modules: ScModuleAbstract) -> None:
        if self.is_registered:
            self._register(*modules)
        self._modules |= {*modules}
        self.logger.info("Added modules: %s", ", ".join(map(repr, modules)))

    def remove_modules(self, *modules: ScModuleAbstract) -> None:
        if self.is_registered:
            self._unregister(*modules)
        self._modules -= {*modules}
        self.logger.info("Removed modules: %s", ", ".join(map(repr, modules)))

    def clear_modules(self) -> None:
        if self.is_registered:
            self._unregister(*self._modules)
        self.logger.info("Removed all modules: %s", ", ".join(map(repr, self._modules)))
        self._modules.clear()

    def register_modules(self) -> _Finisher:
        if self.is_registered:
            self.logger.warning("Modules are already registered")
        else:
            self._register(*self._modules)
            self.is_registered = True
            self.logger.info("Registered modules successfully")
        return _Finisher(self.unregister_modules, self.logger)

    def unregister_modules(self) -> None:
        if not self.is_registered:
            self.logger.warning("Modules are already unregistered")
            return
        self._unregister(*self._modules)
        self.is_registered = False
        self.logger.info("Unregistered modules successfully")

    def start(self) -> _Finisher:
        self.connect()
        self.register_modules()
        return _Finisher(self.stop, self.logger)

    def stop(self) -> None:
        self.unregister_modules()
        self.disconnect()

    def _register(self, *modules: ScModuleAbstract) -> None:
        if not client.is_connected():
            self.logger.error("Failed to register: connection lost")
            raise ConnectionError(f"Connection to url {repr(self._url)} lost")
        for module in modules:
            if not isinstance(module, ScModuleAbstract):
                self.logger.error("Failed to register: type of %s is not ScModule", repr(module))
                raise TypeError(f"{repr(module)} is not ScModule")
            module._register()  # pylint: disable=protected-access

    def _unregister(self, *modules: ScModuleAbstract) -> None:
        if not client.is_connected():
            self.logger.error("Failed to unregister: connection to %s lost", repr(self._url))
            raise ConnectionError(f"Connection to {repr(self._url)} lost")
        for module in modules:
            module._unregister()  # pylint: disable=protected-access

    def serve(self) -> None:
        """Serve agents until a SIGINT signal (^C, or stop in IDE) is received"""

        signal.signal(signal.SIGINT, lambda *_: self.logger.info("^C SIGINT was interrupted"))
        signal.pause()


class _Finisher:
    """Class for calling finish method in with-statement"""

    def __init__(self, finish_method: Callable[[], None], logger: Logger) -> None:
        self._finish_method = finish_method
        self._logger = logger

    def __enter__(self) -> None:
        pass  # Interaction through the beginning method (with server.start_method(): ...)

    def __exit__(self, exc_type, exc_val, exc_tb) -> None:
        if exc_val is not None:
            self._logger.error("Raised error %s, finishing", repr(exc_val))
        self._finish_method()
