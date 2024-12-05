from pathlib import Path

from modules.AudioRecognitionModule.AudioRecognitionModule import AudioRecognitionModule
from sc_client.client import template_search
from sc_client.constants import sc_types
from sc_client.models import ScTemplate
from sc_kpm import ScKeynodes
from sc_kpm.identifiers import CommonIdentifiers
from sc_kpm.utils import get_link_content_data
from sc_kpm.utils.action_utils import execute_agent
from tests.base_testcase import BaseTestCase

from sc_kpm.identifiers import CommonIdentifiers, ActionStatus

WAIT_TIME = 5


class AudioRecognitionAgentTestCase(BaseTestCase):
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

    def run_audio_recognition_agent(self, message_node):
        kwargs = dict(
            arguments={message_node: False},
            concepts=[CommonIdentifiers.ACTION, "action_recognize_audio"],
            reaction=ActionStatus.ACTION_FINISHED_SUCCESSFULLY,
            wait_time=WAIT_TIME,
        )
        self.assertTrue(execute_agent(**kwargs)[1])

    def test_first(self):
        self.load_scs(
            Path(
                self.tests_structures_dir_path,
                "test_audio_recognition_first.scs",
            )
        )
        module = AudioRecognitionModule()
        self.server.add_modules(module)
        
        with self.server.register_modules():
            message_node = ScKeynodes.resolve(
                "test_message1", sc_types.NODE)
            self.assertTrue(message_node.is_valid())
            
            self.run_audio_recognition_agent(message_node)

            text_translation = ScKeynodes.resolve("nrel_sc_text_translation", sc_types.NODE)
            template = ScTemplate()
            template.triple_with_relation(
                sc_types.NODE,
                sc_types.EDGE_D_COMMON_VAR,
                message_node,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                text_translation,
            )
            search_results = template_search(template)
            translation_node = search_results[0][0]
            template = ScTemplate()
            template.triple(
                translation_node,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                sc_types.LINK,
            )
            search_results = template_search(template)
            for result in search_results:
                concept_text_file = ScKeynodes.resolve("concept_text_file", sc_types.NODE)
                template = ScTemplate()
                template.triple(
                    concept_text_file,
                    sc_types.EDGE_ACCESS_VAR_POS_PERM,
                    result[2],
                )
                search_results_for_sound = template_search(template)
                if len(search_results_for_sound) != 0:
                    link = result[2]

            self.assertEqual(get_link_content_data(
                link).lower(), "погода в минске")

        self.server.remove_modules(module)

    def test_second(self):
        self.load_scs(
            Path(
                self.tests_structures_dir_path,
                "test_audio_recognition_second.scs",
            )
        )
        module = AudioRecognitionModule()
        self.server.add_modules(module)

        with self.server.register_modules():
            message_node = ScKeynodes.resolve(
                "test_message2", sc_types.NODE)
            self.assertTrue(message_node.is_valid())
            
            self.run_audio_recognition_agent(message_node)

            text_translation = ScKeynodes.resolve("nrel_sc_text_translation", sc_types.NODE)
            template = ScTemplate()
            template.triple_with_relation(
                sc_types.NODE,
                sc_types.EDGE_D_COMMON_VAR,
                message_node,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                text_translation,
            )
            search_results = template_search(template)
            translation_node = search_results[0][0]
            template = ScTemplate()
            template.triple(
                translation_node,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                sc_types.LINK,
            )
            search_results = template_search(template)
            for result in search_results:
                concept_text_file = ScKeynodes.resolve("concept_text_file", sc_types.NODE)
                template = ScTemplate()
                template.triple(
                    concept_text_file,
                    sc_types.EDGE_ACCESS_VAR_POS_PERM,
                    result[2],
                )
                search_results_for_sound = template_search(template)
                if len(search_results_for_sound) != 0:
                    link = result[2]

            self.assertEqual(get_link_content_data(
                link).lower(), "что такое ника")

        self.server.remove_modules(module)