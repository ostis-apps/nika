import logging
import base64
import io
import speech_recognition as sr
from sc_client.models import ScAddr, ScLinkContentType, ScTemplate, ScLinkContent
from sc_client.constants import sc_types
from sc_client.client import template_search

from sc_kpm import ScAgentClassic, ScResult
from sc_kpm.sc_sets import ScSet
from sc_kpm.utils import (
    create_link,
    get_link_content_data,
    create_edge,
)
from sc_kpm.utils.action_utils import (
    finish_action_with_status,
    get_action_arguments
)
from sc_kpm import ScKeynodes


logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)


class AudioRecognitionAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_recognize_audio")

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        result = self.run(action_element)
        is_successful = result == ScResult.OK
        finish_action_with_status(action_element, is_successful)
        self.logger.info("AudioRecognitionAgent finished %s",
                         "successfully" if is_successful else "unsuccessfully")
        return result

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("AudioRecognitionAgent started")

        try:
            message_addr = get_action_arguments(action_node, 1)[0]
            text_translation = ScKeynodes.resolve("nrel_sc_text_translation", sc_types.NODE)
            template = ScTemplate()
            template.triple_with_relation(                
                sc_types.NODE,
                sc_types.EDGE_D_COMMON_VAR,
                message_addr,
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
                concept_sound_file = ScKeynodes.resolve("concept_sound_file", sc_types.NODE)
                concept_text_file = ScKeynodes.resolve("concept_text_file", sc_types.NODE)
                template = ScTemplate()
                template.triple(
                    concept_sound_file,
                    sc_types.EDGE_ACCESS_VAR_POS_PERM,
                    result[2],
                )
                search_results_for_sound = template_search(template)
                if len(search_results_for_sound) !=0:
                    link = result[2]
                template = ScTemplate()
                template.triple(
                    concept_text_file,
                    sc_types.EDGE_ACCESS_VAR_POS_PERM,
                    result[2],
                )
                search_results_for_sound = template_search(template)
                if len(search_results_for_sound) !=0:
                    self.logger.info(f"AudioRecognitionAgent: already recognized")
                    return ScResult.OK

            base64_text = get_link_content_data(link)
            
            audio_data = base64.b64decode(base64_text) # Декодировать Base64
            audio_stream = io.BytesIO(audio_data) # Создать буферизованный объект для аудиоданных
            recognizer = sr.Recognizer() # Распознавание речи из аудиофайла
            audio_file = sr.AudioFile(audio_stream)
            with audio_file as source:
                audio_data = recognizer.record(source)
                text = recognizer.recognize_google(audio_data, language='ru-RU')
            self.logger.info(f"Google thinks you said:\n {text}")

            link_addr = create_link(text, ScLinkContentType.STRING)
            create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, translation_node, link_addr)
            create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, concept_text_file, link_addr)
            create_edge(sc_types.EDGE_ACCESS_CONST_POS_PERM, ScKeynodes.resolve("lang_ru", sc_types.NODE), link_addr)

        except:
            self.logger.info(f"AudioRecognitionAgent: finished with an error")
            return ScResult.ERROR

        return ScResult.OK
    