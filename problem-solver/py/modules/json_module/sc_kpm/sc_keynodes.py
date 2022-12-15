"""
This source file is part of an OSTIS project. For the latest info, see https://github.com/ostis-ai
Distributed under the MIT License
(See an accompanying file LICENSE or a copy at https://opensource.org/licenses/MIT)
"""

from __future__ import annotations

from logging import Logger, getLogger

from sc_client import client
from sc_client.constants.exceptions import InvalidValueError
from sc_client.constants.sc_types import ScType
from sc_client.models import ScAddr, ScIdtfResolveParams

Idtf = str


class ScKeynodes:
    """
    A singleton dict-like object which provides
    the ability to cache the identifier and ScAddr of keynodes stored in the KB.
    """

    _instance: ScKeynodes = None
    _dict: dict[Idtf, ScAddr] = {}
    _logger: Logger

    def __new__(cls) -> ScKeynodes:
        if cls._instance is None:
            cls._instance = object.__new__(cls)
            cls._logger = getLogger(f"{__name__}.{cls.__name__}")
        return cls._instance

    def __getitem__(self, identifier: Idtf) -> ScAddr:
        """Get keynode, cannot be invalid ScAddr(0)"""
        addr = self.get(identifier)
        if not addr.is_valid():
            self._logger.error("Failed to get ScAddr by %s keynode: ScAddr is invalid", identifier)
            raise InvalidValueError(f"ScAddr of {identifier} is invalid")
        return addr

    def get(self, identifier: Idtf) -> ScAddr:
        """Get keynode, can be ScAddr(0)"""
        return self.resolve(identifier, None)

    def resolve(self, identifier: Idtf, sc_type: ScType | None) -> ScAddr:
        """Get keynode. If sc_type is valid, an element will be created in the KB"""
        addr = self._dict.get(identifier)
        if addr is None or (not addr.is_valid() and sc_type is not None):
            params = ScIdtfResolveParams(idtf=identifier, type=sc_type)
            addr = client.resolve_keynodes(params)[0]
            self._dict[identifier] = addr
            self._logger.debug("Resolved %s identifier with type %s: %s", repr(identifier), repr(sc_type), repr(addr))
        return addr
