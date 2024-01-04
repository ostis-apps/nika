"""
This code creates some test agent and registers until the user stops the process.
For this we wait for SIGINT.
"""
import logging
from sc_client.models import ScAddr, ScLinkContentType, ScTemplate
from sc_client.constants import sc_types
from sc_client.client import template_search

from sc_kpm import ScAgentClassic, ScModule, ScResult, ScServer
from sc_kpm.sc_sets import ScSet
from sc_kpm.utils import (
    create_link,
    create_node,
    get_link_content_data,
    check_edge, create_edge,
    delete_edges,
    get_element_by_role_relation,
    get_element_by_norole_relation,
    get_system_idtf,
    get_edge
)
from sc_kpm.utils.action_utils import (
    create_action_answer,
    finish_action_with_status,
    get_action_arguments,
    get_element_by_role_relation
)
from wikipedia import *
from random import *
from sc_kpm import ScKeynodes

import requests


logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)

class RegisterUserAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_register_user")
        self.logger.info("RegisterUserAgent Open %s")

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        result = self.run(action_element)
        is_successful = result == ScResult.OK
        finish_action_with_status(action_element, is_successful)
        self.logger.info("RegisterUserAgent finished %s",
                         "successfully" if is_successful else "unsuccessfully")
        return result

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("RegisterUserAgent started")

        # Получение введенных почты и пароля
        [email_link_addr, password_link_addr] = get_action_arguments(action_node, 2)
        
        # Проверка на отсутствие регистрации почты
        template = ScTemplate()
        template.triple_with_relation(
            sc_types.NODE_VAR,
            sc_types.EDGE_D_COMMON_VAR,
            email_link_addr,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            ScKeynodes['nrel_email'],
        )
        result = template_search(template)
        if len(result) != 0:
            self.logger.error('RegisterUserAgent: There is email in kb.')
            return ScResult.ERROR

        # Создание пользователя с заданными почтой и паролем
        user_addr = create_node(sc_types.NODE_CONST)
        email_edge = create_edge(sc_types.EDGE_D_COMMON_CONST, user_addr, email_link_addr)
        create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, ScKeynodes['nrel_email'], email_edge)
        password_edge = create_edge(sc_types.EDGE_D_COMMON_CONST, user_addr, password_link_addr)
        create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, ScKeynodes['nrel_password'], password_edge)
        create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, ScKeynodes['concept_users'], user_addr)

        # Добавление пользователя во множество зарегистрированных пользователей
        create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, ScKeynodes['concept_registered_user'], user_addr)
        
        create_action_answer(action_node, user_addr)
        return ScResult.OK