"""
This source file is part of an OSTIS project. For the latest info, see https://github.com/ostis-ai
Distributed under the MIT License
(See an accompanying file LICENSE or a copy at https://opensource.org/licenses/MIT)
"""

import logging


def set_root_config(root_name: str) -> None:
    logging.getLogger(root_name).addHandler(logging.NullHandler())
