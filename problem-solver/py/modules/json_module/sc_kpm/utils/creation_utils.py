"""
This source file is part of an OSTIS project. For the latest info, see https://github.com/ostis-ai
Distributed under the MIT License
(See an accompanying file LICENSE or a copy at https://opensource.org/licenses/MIT)
"""

from sc_client.constants import sc_types
from sc_client.constants.sc_types import ScType
from sc_client.models import ScAddr

from sc_kpm.identifiers import CommonIdentifiers
from sc_kpm.sc_keynodes import ScKeynodes
from sc_kpm.utils.common_utils import create_edge, create_node, create_norole_relation, create_role_relation


def wrap_in_oriented_set(set_node: ScAddr, start_element: ScAddr, *elements: ScAddr) -> None:
    keynodes = ScKeynodes()
    rrel_one = keynodes[CommonIdentifiers.RREL_ONE]
    nrel_sequence = keynodes[CommonIdentifiers.NREL_BASIC_SEQUENCE]
    curr_edge = create_role_relation(set_node, start_element, rrel_one)
    for next_element in elements:
        next_edge = create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, set_node, next_element)
        create_norole_relation(curr_edge, next_edge, nrel_sequence)
        curr_edge = next_edge


def create_oriented_set(*elements: ScAddr) -> ScAddr:
    set_node = create_node(sc_types.NODE_CONST)
    wrap_in_oriented_set(set_node, *elements)
    return set_node


def wrap_in_set(set_node: ScAddr, *elements: ScAddr) -> None:
    for element in elements:
        create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, set_node, element)


def create_set(set_type: ScType, *elements: ScAddr) -> ScAddr:
    set_node = create_node(set_type)
    wrap_in_set(set_node, *elements)
    return set_node


def create_structure(*elements: ScAddr) -> ScAddr:
    return create_set(sc_types.NODE_CONST_STRUCT, *elements)
