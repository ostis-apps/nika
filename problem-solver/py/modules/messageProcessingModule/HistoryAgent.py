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
from sc_kpm import ScKeynodes

import requests
import random

logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)


class HistoryAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_show_history")

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        result = self.run(action_element)
        is_successful = result == ScResult.OK
        finish_action_with_status(action_element, is_successful)
        self.logger.info("HistoryAgent finished %s",
                         "successfully" if is_successful else "unsuccessfully")
        return result

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("HistoryAgent started")

        try:
            message_addr = get_action_arguments(action_node, 1)[0]
            message_type = ScKeynodes.resolve(
                "concept_message_about_history", sc_types.NODE_CONST_CLASS)

            if not check_edge(sc_types.EDGE_ACCESS_VAR_POS_PERM, message_type, message_addr):
                self.logger.info(
                    f"HistoryAgent: the message isn’t about weather")
                return ScResult.OK

            idtf = ScKeynodes.resolve("nrel_idtf", sc_types.NODE_CONST_NOROLE)
            answer_phrase = ScKeynodes.resolve(
                "show_history_answer_phrase", sc_types.NODE_CONST_CLASS)
            rrel_city_place = ScKeynodes.resolve("rrel_city_place", sc_types.NODE_ROLE)
            nrel_history = ScKeynodes.resolve(
                "nrel_history_message", sc_types.NODE_NOROLE)
        except:
            self.logger.info(f"HistoryAgent: finished with an error")
            return ScResult.ERROR
    
        city_addr = self.get_entity_addr(
                message_addr, rrel_city_place)
        city_idtf_link = self.get_ru_idtf(city_addr)
        entity_idtf = get_link_content_data(city_idtf_link)

        self.clear_previous_answer(
                city_addr, nrel_history, answer_phrase)
        try:
            set_lang("ru")
            # history = f'<img src="{page("Город " + entity_idtf + " (Беларусь)").images[0]}<br>">'
            array = page("Город " + entity_idtf + " (Беларусь)").images
            history =  f'<img src="{array[-1]}" style="width: 100%; border-radius: 10px; margin-bottom: 10px;">' + "<br>" + summary("Город " + entity_idtf + " (Беларусь)", sentences = 4)
            self.logger.info(f"HistoryAgent: The temperature in {get_system_idtf(city_addr)} is {history}")
            self.logger.info(f"{history}")
        except requests.exceptions.ConnectionError:
            self.logger.info(f"HistoryAgent: finished with connection error")
            return ScResult.ERROR
        link = create_link(
            str(history), ScLinkContentType.STRING, link_type=sc_types.LINK_CONST)
        nrel_format = ScKeynodes.resolve(
                "nrel_format", sc_types.NODE_NOROLE)
        format_html = ScKeynodes.resolve(
                "format_html", sc_types.NODE_CONST)
        format_edge = create_edge(
            sc_types.EDGE_D_COMMON_CONST, link, format_html)
        create_edge(
            sc_types.EDGE_ACCESS_CONST_POS_PERM, nrel_format, format_edge)
        
        history_edge = create_edge(
            sc_types.EDGE_D_COMMON_CONST, city_addr, link)
        create_edge(
            sc_types.EDGE_ACCESS_CONST_POS_PERM, nrel_history, history_edge)
        create_action_answer(action_node, link)

        return ScResult.OK

    def set_unknown_city_link(self, action_node: ScAddr, answer_phrase: ScAddr) -> None:
        unknown_city_link = ScKeynodes.resolve(
            "unknown_city_for_weather_agent_message_text", None)
        if not unknown_city_link.is_valid():
            raise
        create_edge(
            sc_types.EDGE_ACCESS_CONST_POS_PERM, answer_phrase, unknown_city_link)
        create_action_answer(action_node, unknown_city_link)

    def get_ru_idtf(self, entity_addr: ScAddr) -> ScAddr:
        main_idtf = ScKeynodes.resolve(
            "nrel_main_idtf", sc_types.NODE_CONST_NOROLE)
        lang_ru = ScKeynodes.resolve("lang_ru", sc_types.NODE_CONST_CLASS)

        template = ScTemplate()
        template.triple_with_relation(
            entity_addr,
            sc_types.EDGE_D_COMMON_VAR,
            sc_types.LINK,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            main_idtf,
        )
        search_results = template_search(template)
        for result in search_results:
            idtf = result[2]
            lang_edge = get_edge(
                lang_ru, idtf, sc_types.EDGE_ACCESS_VAR_POS_PERM)
            if lang_edge:
                return idtf
        return get_element_by_norole_relation(
            src=entity_addr, nrel_node=main_idtf)


    def get_entity_addr(self, message_addr: ScAddr, rrel_entity: ScAddr):
        template = ScTemplate()
        # entity node or link
        template.triple_with_relation(
            message_addr,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            sc_types.VAR,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            rrel_entity,
        )
        search_results = template_search(template)
        if len(search_results) == 0:
            return ScAddr(0)
        entity = search_results[0][2]
        if len(search_results) == 1:
            return entity


    def clear_previous_answer(self, entity, nrel_history, answer_phrase):
        message_answer_set = ScSet(set_node=answer_phrase)
        message_answer_set.clear()
        if not entity.is_valid():
            return

        template = ScTemplate()
        template.triple_with_relation(
            entity,
            sc_types.EDGE_D_COMMON_VAR,
            sc_types.LINK,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            nrel_history
        )
        search_results = template_search(template)
        for result in search_results:
            delete_edges(result[0], result[2], sc_types.EDGE_D_COMMON_VAR)
