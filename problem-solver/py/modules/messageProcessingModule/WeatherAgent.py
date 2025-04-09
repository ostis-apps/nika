"""
This code creates some test agent and registers until the user stops the process.
For this we wait for SIGINT.
"""
import logging
from sc_client.models import ScAddr, ScLinkContentType, ScTemplate
from sc_client.constants import sc_type
from sc_client.client import search_by_template

from sc_kpm import ScAgentClassic, ScResult
from sc_kpm.sc_sets import ScSet
from sc_kpm.utils import (
    generate_link,
    get_link_content_data,
    check_connector, generate_connector,
    erase_connectors,
    search_element_by_non_role_relation,
    get_element_system_identifier,
    search_connector
)
from sc_kpm.utils.action_utils import (
    generate_action_result,
    finish_action_with_status,
    get_action_arguments,
)
from sc_kpm import ScKeynodes

import requests


logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)


class WeatherAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_show_weather")

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        result = self.run(action_element)
        is_successful = result == ScResult.OK
        finish_action_with_status(action_element, is_successful)
        self.logger.info("WeatherAgent finished %s",
                         "successfully" if is_successful else "unsuccessfully")
        return result

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("WeatherAgent started")

        try:
            message_addr = get_action_arguments(action_node, 1)[0]
            message_type = ScKeynodes.resolve(
                "concept_message_about_weather", sc_type.CONST_NODE_CLASS)

            if not check_connector(sc_type.VAR_PERM_POS_ARC, message_type, message_addr):
                self.logger.info(
                    f"WeatherAgent: the message isn’t about weather")
                return ScResult.OK

            idtf = ScKeynodes.resolve("nrel_idtf", sc_type.CONST_NODE_NON_ROLE)
            answer_phrase = ScKeynodes.resolve(
                "show_weather_answer_phrase", sc_type.CONST_NODE_CLASS)
            rrel_entity = ScKeynodes.resolve("rrel_entity", sc_type.CONST_NODE_ROLE)
            nrel_temperature = ScKeynodes.resolve(
                "nrel_temperature", sc_type.CONST_NODE_NON_ROLE)

            city_addr, country_addr = self.get_entity_addr(
                message_addr, rrel_entity)

            self.clear_previous_answer(
                city_addr, nrel_temperature, answer_phrase)

            # if there is no such сity in country
            if not country_addr is None:
                if not search_connector(country_addr, city_addr, sc_type.VAR_COMMON_ARC):
                    self.set_unknown_city_link(action_node, answer_phrase)
                    return ScResult.OK

            # if there is no such сity
            if not city_addr.is_valid():
                self.set_unknown_city_link(action_node, answer_phrase)
                return ScResult.OK
            city_idtf_link = self.get_ru_main_identifier(city_addr)
            answer_city_idtf_link = search_element_by_non_role_relation(
                src=city_addr, nrel_node=idtf)
            if not city_idtf_link.is_valid():
                self.set_unknown_city_link(action_node, answer_phrase)
                return ScResult.OK
        except Exception as e:
            self.logger.info(f"WeatherAgent: finished with an error {e}")
            return ScResult.ERROR

        entity_idtf = get_link_content_data(city_idtf_link)
        try:
            temperature = self.get_weather(
                entity_idtf, city_addr, country_addr)
        except requests.exceptions.ConnectionError:
            self.logger.info(f"WeatherAgent: finished with connection error")
            return ScResult.ERROR
        link = generate_link(
            str(temperature), ScLinkContentType.STRING, link_type=sc_type.CONST_NODE_LINK)
        temperature_edge = generate_connector(
            sc_type.CONST_COMMON_ARC, city_addr, link)
        generate_connector(
            sc_type.CONST_PERM_POS_ARC, nrel_temperature, temperature_edge)
        generate_action_result(action_node, link)

        return ScResult.OK

    def get_weather(self, entity_idtf: ScAddr, city_addr: ScAddr, country_addr: ScAddr) -> float:
        # get entity longitude and latitude
        if country_addr is not None:
            country = get_link_content_data(self.get_ru_main_identifier(country_addr))
            coordinates = requests.get(
                f'https://geocode.maps.co/search?city={entity_idtf}&country={country}').json()[0]
        else:
            coordinates = requests.get(
                f'https://geocode.maps.co/search?city={entity_idtf}').json()[0]

        # get weather
        response = requests.get(
            "https://api.open-meteo.com/v1/forecast?"
            f"latitude={coordinates['lat']}&longitude={coordinates['lon']}&current_weather=true")

        weather_data = response.json()
        main_data = weather_data["current_weather"]
        temperature = main_data["temperature"]
        self.logger.info(
            f"WeatherAgent: The temperature in {get_element_system_identifier(city_addr)} is {temperature}°C")
        return temperature

    def set_unknown_city_link(self, action_node: ScAddr, answer_phrase: ScAddr) -> None:
        unknown_city_link = ScKeynodes.resolve(
            "unknown_city_for_weather_agent_message_text", None)
        if not unknown_city_link.is_valid():
            raise
        generate_connector(
            sc_type.CONST_PERM_POS_ARC, answer_phrase, unknown_city_link)
        generate_action_result(action_node, unknown_city_link)

    def get_ru_main_identifier(self, entity_addr: ScAddr) -> ScAddr:
        main_idtf = ScKeynodes.resolve(
            "nrel_main_idtf", sc_type.CONST_NODE_NON_ROLE)
        lang_ru = ScKeynodes.resolve("lang_ru", sc_type.CONST_NODE_CLASS)

        template = ScTemplate()
        template.quintuple(
            entity_addr,
            sc_type.VAR_COMMON_ARC,
            sc_type.VAR_NODE_LINK,
            sc_type.VAR_PERM_POS_ARC,
            main_idtf,
        )
        search_results = search_by_template(template)
        for result in search_results:
            idtf = result[2]
            lang_edge = search_connector(
                lang_ru, idtf, sc_type.VAR_PERM_POS_ARC)
            if lang_edge:
                return idtf
        return search_element_by_non_role_relation(
            src=entity_addr, nrel_node=main_idtf)

    def get_entity_addr(self, message_addr: ScAddr, rrel_entity: ScAddr):
        rrel_entity = ScKeynodes.resolve("rrel_entity", sc_type.CONST_NODE_ROLE)
        concept_country = ScKeynodes.resolve(
            "concept_country", sc_type.CONST_NODE_CLASS)
        template = ScTemplate()
        # entity node or link
        template.quintuple(
            message_addr,
            sc_type.VAR_PERM_POS_ARC,
            sc_type.VAR,
            sc_type.VAR_PERM_POS_ARC,
            rrel_entity,
        )
        search_results = search_by_template(template)
        if len(search_results) == 0:
            return ScAddr(0), None
        entity = search_results[0][2]
        if len(search_results) == 1:
            return entity, None
        # check country position in search_results
        country_edge = search_connector(
            concept_country, entity, sc_type.VAR_PERM_POS_ARC)
        if country_edge:
            return search_results[1][2], entity
        else:
            return entity, search_results[1][2]

    def clear_previous_answer(self, entity, nrel_temperature, answer_phrase):
        message_answer_set = ScSet(set_node=answer_phrase)
        message_answer_set.clear()
        if not entity.is_valid():
            return

        template = ScTemplate()
        template.quintuple(
            entity,
            sc_type.VAR_COMMON_ARC,
            sc_type.VAR_NODE_LINK,
            sc_type.VAR_PERM_POS_ARC,
            nrel_temperature
        )
        search_results = search_by_template(template)
        for result in search_results:
            erase_connectors(result[0], result[2], sc_type.VAR_COMMON_ARC)
