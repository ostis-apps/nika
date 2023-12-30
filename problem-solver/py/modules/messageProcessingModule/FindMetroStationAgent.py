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
API_KEY = '5b3ce3597851110001cf62484e61be75f1be4fd19569f26fa1371ce0'


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
        [message_addr] = get_action_arguments(action_node, 1)
        rrel_first_place = ScKeynodes.resolve('rrel_first_place', sc_types.NODE_CONST_ROLE)
        rrel_second_place = ScKeynodes.resolve('rrel_second_place', sc_types.NODE_CONST_ROLE)

        # Получение данных (места от которого и до которого надо добраться)
        first_place_link_addr = self.get_entity_addr(message_addr, rrel_first_place)
        second_place_link_addr = self.get_entity_addr(message_addr, rrel_second_place)
        first_place = get_link_content_data(first_place_link_addr)
        second_place = get_link_content_data(second_place_link_addr)

        # Получение координат мест
        try:
            response1 = requests.get(f"https://api.openrouteservice.org/geocode/search?api_key={API_KEY}&text={first_place}&sources=openstreetmap,openaddresses,geonames,whosonfirst&boundary.rect.min_lat={minx}&boundary.rect.max_lat={maxx}&boundary.rect.min_lon={miny}&boundary.rect.max_lon={maxy}").json()
            response2 = requests.get(f"https://api.openrouteservice.org/geocode/search?api_key={API_KEY}&text={second_place}&sources=openstreetmap,openaddresses,geonames,whosonfirst&boundary.rect.min_lat={minx}&boundary.rect.max_lat={maxx}&boundary.rect.min_lon={miny}&boundary.rect.max_lon={maxy}").json()
        except:
            answer = choice([f'К сожалению, не удалось распознать, куда вам надо.'])

            link = create_link(
                answer, ScLinkContentType.STRING, link_type=sc_types.LINK_CONST)
            create_action_answer(action_node, link)
            return ScResult.OK
    
        ip_first_place = f"{response1['features'][0]['geometry']['coordinates'][0]}, {response1['features'][0]['geometry']['coordinates'][1]}"
        ip_second_place = f"{response2['features'][0]['geometry']['coordinates'][0]}, {response2['features'][0]['geometry']['coordinates'][1]}"

        # Поиск ближайших точек входа и выхода в метро
        concept_mestro_station = ScKeynodes.resolve('concept_mestro_station', sc_types.NODE_CONST_CLASS)
        
        template = ScTemplate()
        template.triple(
            concept_mestro_station,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            sc_types.NODE_VAR << "_station",
        )

        template.triple_with_relation(
            '_station',
            sc_types.EDGE_D_COMMON_VAR,
            sc_types.NODE_VAR << "_station_inputs",
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            ScKeynodes['nrel_inputs'],
        )

        template.triple(
            '_station_inputs',
            sc_types.EDGE_D_COMMON_VAR,
            sc_types.NODE_VAR << "_station_input",
        )

        template.triple_with_relation(
            '_station_input',
            sc_types.EDGE_D_COMMON_VAR,
            sc_types.LINK_VAR << "_coordinates",
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            ScKeynodes['nrel_coordinates'],
        )

        result = template_search(template)

        if len(result) == 0:
            self.logger.error('FindMetroStationAgent: There are no stations.')
            return ScResult.ERROR

        min_d_in = 9999999
        station_in = ''
        min_d_out = 9999999
        station_outs = ''

        for metro_input in result:
            coordinates_addr = metro_input.get('_coordinates')
            coordinates = get_link_content_data(coordinates_addr)
            metro_input_addr = metro_input.get('_station_input')

            # Получение расстояний до точки 
            d_in = requests.get(f"https://api.openrouteservice.org/v2/directions/driving-car?api_key={API_KEY}&start={ip_first_place}&end={coordinates}").json()['features'][0]['properties']['segments'][0]['distance']
            d_out= requests.get(f"https://api.openrouteservice.org/v2/directions/driving-car?api_key={API_KEY}&start={coordinates}&end={ip_second_place}").json()['features'][0]['properties']['segments'][0]['distance']
            

            if d_in < min_d_in:
                min_d_in = d_in
                station_in = metro_input_addr

            if d_out < min_d_out:
                min_d_out = d_out
                station_out = metro_input_addr

        # Оформление вывода
        answer = choice([f'Для того, чтобы добраться от {first_place} до {second_place} вым необходимо пройти к станции {self.get_ru_idtf(station_in)} и ехать до {self.get_ru_idtf(station_out)}'])

        link = create_link(
            answer, ScLinkContentType.STRING, link_type=sc_types.LINK_CONST)
        create_action_answer(action_node, link)
        return ScResult.OK

    def get_entity_addr(self, message_addr: ScAddr, rrel_entity: ScAddr):
        
        template = ScTemplate()
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
        
        return search_results[0][2]
    
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
    