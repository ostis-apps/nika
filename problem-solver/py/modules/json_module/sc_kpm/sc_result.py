"""
This source file is part of an OSTIS project. For the latest info, see https:#github.com/ostis-ai
Distributed under the MIT License
(See an accompanying file LICENSE or a copy at https:#opensource.org/licenses/MIT)
"""

from enum import IntEnum, auto


class ScResult(IntEnum):
    ERROR = 0  # unknown error, should be 0 anytime
    OK = 1  # no errors, should be 1 anytime
    ERROR_INVALID_PARAMS = auto()  # invalid function parameters error
    ERROR_INVALID_TYPE = auto()  # invalid type error
    ERROR_IO = auto()  # input/output error
    ERROR_INVALID_STATE = auto()  # invalid state of processed object
    ERROR_NOT_FOUND = auto()  # item not found
    ERROR_NO_WRITE_RIGHTS = auto()  # no rights to change or delete object
    ERROR_NO_READ_RIGHTS = auto()  # no rights to read object
    NO = auto()  # no result
    UNKNOWN = auto()  # result unknown
    SKIP = auto()  # skip agent
