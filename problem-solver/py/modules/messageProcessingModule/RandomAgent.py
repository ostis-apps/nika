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
from random import *
from sc_kpm import ScKeynodes

import requests


logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)


class RandomAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_show_random_city")
        self.logger.info("RandomAgent Open %s")

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        result = self.run(action_element)
        is_successful = result == ScResult.OK
        finish_action_with_status(action_element, is_successful)
        self.logger.info("RandomAgent finished %s",
                         "successfully" if is_successful else "unsuccessfully")
        return result

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("RandomAgent started")

        try:
            message_addr = get_action_arguments(action_node, 1)[0]
            message_type = ScKeynodes.resolve(
                "concept_message_about_random_city", sc_types.NODE_CONST_CLASS)

            if not check_edge(sc_types.EDGE_ACCESS_VAR_POS_PERM, message_type, message_addr):
                self.logger.info(
                    f"RandomAgent: the message isn’t about random city")
                return ScResult.OK

            idtf = ScKeynodes.resolve("nrel_idtf", sc_types.NODE_CONST_NOROLE)
            answer_phrase = ScKeynodes.resolve(
                "show_random_city_answer_phrase", sc_types.NODE_CONST_CLASS)
            nrel_random_city = ScKeynodes.resolve(
                "nrel_random_city", sc_types.NODE_NOROLE)
        except:
            self.logger.info(f"RandomAgent: finished with an error")
            return ScResult.ERROR

        # Прописаны все города
        template = ScTemplate()
        template.triple(
            ScKeynodes['concept_city_place'],
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            sc_types.NODE_VAR >> "_city_addr",
        )
        result = template_search(template)

        no_city = False
        cities = []
        for item in result:
            cities.append(item.get("_city_addr"))

        print(cities)

        if len(cities) == 0:
            no_city = True
            cities = ['Минск', 'Борисов', 'Солигорск', 'Молодечно', 'Жодино', 'Слуцк', 'Дзержинск', 'Вилейка',
                      'Смолевичи', 'Марьина Горка', 'Фаниполь', 'Столбцы', 'Заславль', 'Несвиж', 'Логойск', 'Березино',
                      'Любань', 'Клецк', 'Старые Дороги', 'Узда', 'Червень', 'Копыль', 'Воложин', 'Крупки', 'Мядель',
                      'Витебск', 'Орша', 'Новополоцк', 'Полоцк', 'Поставы', 'Глубокое', 'Лепель', 'Мир', 'Новолукомль',
                      'Городок', 'Барань', 'Толочин', 'Браслав', 'Чашники', 'Миоры', 'Сенно', 'Верхнедвинск',
                      'Дубровно', 'Докшицы', 'Дисна', 'Могилев', 'Бобруйск', 'Осиповичи', 'Горки', 'Кричев', 'Быхов',
                      'Климовичи', 'Костюковичи', 'Шклов', 'Чаусы', 'Мстиславь', 'Белыничи', 'Кировск', 'Чериков',
                      'Славгород', 'Круглое', 'Кличев', 'Гомель', 'Мозырь', 'Жлобин', 'Речица', 'Светлогорск',
                      'Калинковичи', 'Рогачев', 'Добруш', 'Житковичи', 'Хойники', 'Петриков', 'Ельск', 'Чечерск',
                      'Буда-Кошелево', 'Ветка', 'Наровля', 'Василевичи', 'Туров', 'Брест', 'Барановичи', 'Пинск',
                      'Кобрин', 'Береза', 'Лунинец', 'Ивацевичи', 'Пружаны', 'Иваново', 'Дрогичин', 'Жабинка', 'Столин',
                      'Ганцевичи', 'Малорита', 'Микашевичи', 'Белоозерск', 'Ляховичи', 'Каменецк', 'Давид-Городок',
                      'Высокое', 'Коссово', 'Гродно', 'Лида', 'Слоним', 'Волковыск', 'Сморгонь', 'Новогрудок', 'Ошмяны',
                      'Щучин', 'Островец', 'Мосты', 'Скидель', 'Березовка', 'Дятлово', 'Ивье', 'Свислочь']

        if not no_city:
            city_addr = choice(cities)

            template = ScTemplate()
            template.triple_with_relation(
                city_addr,
                sc_types.EDGE_D_COMMON_VAR,
                sc_types.LINK_VAR >> '_description',
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                ScKeynodes['nrel_description'],
            )

            result = template_search(template)
        else:
            result = []

        if not len(result) == 0:
            city_idtf = get_link_content_data(self.get_ru_idtf(city_addr))
            try:
                set_lang("ru")
                print(1)
                array = page("Город " + city_idtf + " (Беларусь)").images
                print(2)
                description1 = f'<img src="{array[0]}" style="width: 100%; border-radius: 10px; margin-bottom: 10px;">' + "<br>" + summary(
                    "Город " + city_idtf + " (Беларусь)", sentences=4)
                print(3)
            except:
                print('Error')

            description_link_addr = result[0].get('_description')
            description = description1 + '<br><br>Интересно:' + (get_link_content_data(description_link_addr).split("Интересно:")[-1])
        else:

            if no_city:
                city = choice(cities)
            else:
                city_link_idtf = self.get_ru_idtf(city_addr)
                city = get_link_content_data(city_link_idtf)
            print(city)

            set_lang("ru")
            try:
                array = page(f"Город {city} (Беларусь)").images
                description = f'<img src="{array[-1]}" style="width: 100%; border-radius: 10px; margin-bottom: 10px;">' + "<br>" + f"<b><p style='text-align: center'>{city}</p></b>" + summary("Город " + city + " (Беларусь)", sentences = 4)
            except:
                array = page(city).images
                description = f'<img src="{array[-1]}" style="width: 100%; border-radius: 10px; margin-bottom: 10px;">' + "<br>" + f"<b><p style='text-align: center'>{city}</p></b>" + summary(city, sentences = 4)

        self.logger.info(f"RandomAgent: {description}")

        link = create_link(
            description, ScLinkContentType.STRING, link_type=sc_types.LINK_CONST)
        edge = create_edge(sc_types.EDGE_D_COMMON_CONST, message_addr, link)
        create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, ScKeynodes['nrel_answer'], edge)

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


    def clear_previous_answer(self, entity, nrel_random_city, answer_phrase):
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
            nrel_random_city
        )
        search_results = template_search(template)
        for result in search_results:
            delete_edges(result[0], result[2], sc_types.EDGE_D_COMMON_VAR)