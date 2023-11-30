"""
This code creates some test agent and registers until the user stops the process.
For this we wait for SIGINT.
"""
import logging
from sc_client.models import ScAddr, ScLinkContentType, ScTemplate
from sc_client.constants import sc_types
from sc_client.client import template_search
from random import choice, random
from math import floor

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
from sc_kpm import ScKeynodes

import requests
from random import randint


logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)


class TravellingAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_show_travelling")

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        try:
            result = self.run(action_element)
        except:
            self.logger.info(f"TravellingAgent: finished with an error")
            return ScResult.ERROR

        is_successful = result == ScResult.OK
        finish_action_with_status(action_element, is_successful)
        self.logger.info("TravellingAgent finished %s",
                         "successfully" if is_successful else "unsuccessfully")
        return result

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("TravellingAgent started")

        message_addr = get_action_arguments(action_node, 1)[0]
        message_type = ScKeynodes.resolve("concept_message_about_travelling", sc_types.NODE_CONST_CLASS)

        if not check_edge(sc_types.EDGE_ACCESS_VAR_POS_PERM, message_type, message_addr):
            self.logger.info(
                f"TravellingAgent: the message isn't about weather")
            return ScResult.OK

        city_addr = self.get_entity_addr(message_addr)
        city_idtf_link = self.get_ru_idtf(city_addr)
        city_idtf = get_link_content_data(city_idtf_link)

        # Будет браться у пользователя
        # desiers = ['театр', "кинотеатр", "заповедники", "парки",  "библиотека", "музей-усадьба"]
        concept_user = ScKeynodes.resolve("concept_tourist", sc_types.NODE_NOROLE)
        nrel_user_desiers = ScKeynodes.resolve("nrel_user_desiers", sc_types.NODE_NOROLE)
        template = ScTemplate()
        template.triple(concept_user, sc_types.EDGE_ACCESS_VAR_POS_PERM, sc_types.NODE_VAR >> "_user")
        template.triple_with_relation(
            "_user",
            sc_types.EDGE_D_COMMON_VAR,
            sc_types.NODE_VAR >> "_desiers",
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            nrel_user_desiers,
        )
        template.triple("_desiers", sc_types.EDGE_ACCESS_VAR_POS_PERM, sc_types.NODE_VAR >> "_desier")
        search_results = template_search(template)

        desiers = []
        for result in search_results:
            disier = result.get("_desier")
            idtf_link = self.get_ru_idtf(disier)
            idtf = get_link_content_data(idtf_link)
            desiers.append(idtf)

        lat = []
        lon = []
        name = []
        adr = []
        attractions = '<br><br>'

        desiers = ['pools', 'fast_food', 'cinemas', 'historical_places', 'concert_halls', 'amusements']
        try:
            coordinates = requests.get(
                    f'https://geocode.maps.co/search?city={city_idtf}&country=Беларусь').json()[0]

            coordString = ''
            for item in desiers:
                try:
                    print(item)
                    api_key = "5ae2e3f221c38a28845f05b6c5d9bf667efa63f94dcb0e435b058e95"
                    places = requests.get(
                        f"https://api.opentripmap.com/0.1/ru/places/radius?radius={2000}&lon={coordinates['lon']}&lat={coordinates['lat']}&kinds={item},&apikey={api_key}"
                    ).json()['features']


                    l = places
                    j = 0
                    f = False

                    while places[j]['properties']['name'] == '':
                        j += 1

                        if j >= len(places):
                            f = True
                            break

                    if f:
                        continue
                    name = places[j]['properties']['name'] 
                    places = places[j]['properties']['xid']

                    inf = requests.get(f'https://api.opentripmap.com/0.1/ru/places/xid/{places}?apikey={api_key}').json()
                    
                    lon.append(inf['point']['lon'])
                    lat.append(inf['point']['lat'])
                    
                    attractions += f"~ {name}"

                    try:
                        attractions += f"<p style='opacity: 0.7'>{inf['address']['road']} {inf['address']['house_number']}</p>"
                    except:
                        attractions += ""
                except:
                    print("~ ERROR ~")

            for i in range(0, len(lat)):
                coordString += str(lon[i]) + ',' + str(lat[i]) + ','
                    
            attractions += '<style>a:hover {background\: rgb(96, 178, 202);}</style><a class="build_map" href="http://localhost:3033/map?x=' + str(coordinates["lon"]) + "," + str(coordinates["lat"]) + "," + coordString + '" style="transition: all .6s ease; display: inline-block; padding: 10px 20px; margin: auto; background: blue; background: #262626; text-decoration: none; border-radius: 10px;">Построить карту</a>'

        except requests.exceptions.ConnectionError:
            self.logger.info(f"TravellingAgent: finished with connection error")
            return ScResult.ERROR
        self.logger.info(f"{attractions}")

        nrel_attractions = ScKeynodes.resolve("nrel_attraction", sc_types.NODE_NOROLE)
        self.clear_previous_answer(city_addr, nrel_attractions)

        link = create_link(str(attractions), ScLinkContentType.STRING, link_type=sc_types.LINK_CONST)
        attractions_edge = create_edge(sc_types.EDGE_D_COMMON_CONST, city_addr, link)
        create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, nrel_attractions, attractions_edge)
        create_action_answer(action_node, link)

        return ScResult.OK

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

    def get_entity_addr(self, message_addr: ScAddr):
        rrel_entity = ScKeynodes.resolve("rrel_city_place", sc_types.NODE_ROLE)
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

    def clear_previous_answer(self, entity, nrel_attractions):
        template = ScTemplate()
        template.triple_with_relation(
            entity,
            sc_types.EDGE_D_COMMON_VAR,
            sc_types.LINK,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            nrel_attractions
        )
        search_results = template_search(template)
        for result in search_results:
            delete_edges(result[0], result[2], sc_types.EDGE_D_COMMON_VAR)
