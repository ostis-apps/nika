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

class AuthoriseUserAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_authorise_user")
        self.logger.info("AuthoriseUserAgent Open %s")

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        result = self.run(action_element)
        is_successful = result == ScResult.OK
        finish_action_with_status(action_element, is_successful)
        self.logger.info("AuthoriseUserAgent finished %s",
                         "successfully" if is_successful else "unsuccessfully")
        return result

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("AuthoriseUserAgent started")

        # Получение введенных почты и пароля
        [email_link_addr, password_link_addr] = get_action_arguments(action_node, 2)
        
        # Поиск пользователя в бз
        template = ScTemplate()
        template.triple_with_relation(
            sc_types.NODE_VAR >> '_user_addr',
            sc_types.EDGE_D_COMMON_VAR,
            email_link_addr,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            ScKeynodes['nrel_email'],
        )
        result = template.template_search()
        if len(result) == 0:
            self.logger.error('AuthoriseUserAgent: There is no user with such email in kb.')
            return ScResult.ERROR
        user_addr = result[0]['_user_addr']

        template = ScTemplate()
        template.triple_with_relation(
            user_addr,
            sc_types.EDGE_D_COMMON_VAR,
            password_link_addr,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            ScKeynodes['nrel_password'],
        )
        result = template.template_search()
        if len(result) == 0:
            self.logger.error('AuthoriseUserAgent: There is no user with such password in kb.')
            return ScResult.ERROR
        
        # Добавление пользователя в класс авторизированных пользователей
        create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, ScKeynodes['concept_authorised_user'], user_addr)
        
        
        
        create_action_answer(action_node, user_addr)
        return ScResult.OK
