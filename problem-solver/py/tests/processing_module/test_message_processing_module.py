from pathlib import Path

from modules.messageProcessingModule.MessageProcessingModule import MessageProcessingModule
from sc_client.client import template_search
from sc_client.constants import sc_types
from sc_client.models import ScTemplate, ScAddr
from sc_kpm import ScKeynodes
from sc_kpm.identifiers import CommonIdentifiers
from sc_kpm.utils import get_edge, get_link_content_data
from sc_kpm.utils.action_utils import execute_agent
from tests.base_testcase import BaseTestCase

from sc_kpm.identifiers import CommonIdentifiers, ActionStatus

WAIT_TIME = 5


class WeatherAgentTestCase(BaseTestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.tests_structures_dir_path = str(
            Path(Path(__file__).resolve().parent, "test_structures"))
        
    def setUp(self):
        super().setUp()
        self.load_scs(
            Path(
                self.tests_structures_dir_path,
                "common.scs",
            )
        )

    def run_weather_agent(self, message_node):
        kwargs = dict(
            arguments={message_node: False},
            concepts=[CommonIdentifiers.ACTION, "action_show_weather"],
            reaction=ActionStatus.ACTION_FINISHED_SUCCESSFULLY,
            wait_time=WAIT_TIME,
        )
        self.assertTrue(execute_agent(**kwargs)[1])

    def test_message_is_not_about_weather(self):
        self.load_scs(
            Path(
                self.tests_structures_dir_path,
                "test_message_is_not_about_weather.scs",
            )
        )
        module = MessageProcessingModule()
        self.server.add_modules(module)
        with self.server.register_modules():
            message_node = ScKeynodes.resolve(
                "test_message", sc_types.NODE)
            self.assertTrue(message_node.is_valid())

            self.run_weather_agent(message_node)

        self.server.remove_modules(module)

    def test_generate_message_by_unknown_city(self):
        self.load_scs(
            Path(
                self.tests_structures_dir_path,
                "test_generate_message_by_unknown_city.scs",
            )
        )
        module = MessageProcessingModule()
        self.server.add_modules(module)
        with self.server.register_modules():
            message_node = ScKeynodes.resolve(
                "test_message", sc_types.NODE)
            self.assertTrue(message_node.is_valid())

            self.run_weather_agent(message_node)

            answer_node = ScKeynodes.resolve(
                "show_weather_answer_phrase", sc_types.NODE)
            template = ScTemplate()
            template.triple(
                answer_node,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                sc_types.LINK,
            )
            search_results = template_search(template)

            self.assertEqual(len(search_results), 1)
            link = search_results[0][2]
            self.assertEqual(get_link_content_data(
                link), "Я не знаю такого города")

        self.server.remove_modules(module)

    def test_generate_message_by_unknown_country(self):
        self.load_scs(
            Path(
                self.tests_structures_dir_path,
                "test_generate_message_by_unknown_country.scs",
            )
        )
        module = MessageProcessingModule()
        self.server.add_modules(module)
        with self.server.register_modules():
            message_node = ScKeynodes.resolve(
                "test_message", sc_types.NODE)
            self.assertTrue(message_node.is_valid())

            self.run_weather_agent(message_node)

            answer_node = ScKeynodes.resolve(
                "show_weather_answer_phrase", sc_types.NODE)
            template = ScTemplate()
            template.triple(
                answer_node,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                sc_types.LINK,
            )
            search_results = template_search(template)

            self.assertEqual(len(search_results), 1)
            link = search_results[0][2]
            self.assertEqual(get_link_content_data(
                link), "Я не знаю такого города")

        self.server.remove_modules(module)

    def test_generate_message_by_known_city(self):
        self.load_scs(
            Path(
                self.tests_structures_dir_path,
                "test_generate_message_by_known_city.scs",
            )
        )
        module = MessageProcessingModule()
        self.server.add_modules(module)
        with self.server.register_modules():
            message_node = ScKeynodes.resolve(
                "test_message", sc_types.NODE)
            self.assertTrue(message_node.is_valid())

            self.run_weather_agent(message_node)

            rrel_entity = ScKeynodes.resolve("rrel_entity", sc_types.NODE_ROLE)
            template = ScTemplate()
            template.triple_with_relation(
                message_node,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                sc_types.NODE,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                rrel_entity
            )
            search_results = template_search(template)
            self.assertEqual(len(search_results), 1)

            entity = search_results[0][2]
            nrel_temperature = ScKeynodes.resolve(
                "nrel_temperature", sc_types.NODE_NOROLE)

            template = ScTemplate()
            template.triple_with_relation(
                entity,
                sc_types.EDGE_D_COMMON_VAR,
                sc_types.LINK,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                nrel_temperature
            )
            search_results = template_search(template)
            self.assertEqual(len(search_results), 1)
        self.server.remove_modules(module)

    def test_generate_message_by_known_city_and_country(self):
        self.load_scs(
            Path(
                self.tests_structures_dir_path,
                "test_generate_message_by_known_city_and_country.scs",
            )
        )
        module = MessageProcessingModule()
        self.server.add_modules(module)
        with self.server.register_modules():
            message_node = ScKeynodes.resolve(
                "test_message_city_and_country", sc_types.NODE)
            self.assertTrue(message_node.is_valid())

            self.run_weather_agent(message_node)

            rrel_entity = ScKeynodes.resolve("rrel_entity", sc_types.NODE_ROLE)
            template = ScTemplate()
            template.triple_with_relation(
                message_node,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                sc_types.NODE,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                rrel_entity
            )
            search_results = template_search(template)
            self.assertEqual(len(search_results), 2)

            concept_country = ScKeynodes.resolve(
                "concept_country", sc_types.NODE)
            country_edge = get_edge(
                concept_country, search_results[0][2], sc_types.EDGE_ACCESS_VAR_POS_PERM)
            if country_edge:
                entity = search_results[1][2]
            else:
                entity = search_results[0][2]

            nrel_temperature = ScKeynodes.resolve(
                "nrel_temperature", sc_types.NODE_NOROLE)

            template = ScTemplate()
            template.triple_with_relation(
                entity,
                sc_types.EDGE_D_COMMON_VAR,
                sc_types.LINK,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                nrel_temperature
            )
            search_results = template_search(template)
            self.assertEqual(len(search_results), 1)

        self.server.remove_modules(module)
