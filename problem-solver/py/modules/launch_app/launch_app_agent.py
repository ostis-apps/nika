import logging
import subprocess

from sc_client.models import ScAddr
from sc_kpm import ScAgentClassic, ScKeynodes, ScResult
from sc_kpm.utils import (
    get_link_content_data,
    search_element_by_role_relation,
)
from sc_kpm.utils.action_utils import (
    finish_action_with_status,
    get_action_arguments,
)


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class LaunchAppAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_launch_app")
        self.rrel_program = ScKeynodes.get("rrel_program")

    def on_event(
        self,
        event_element: ScAddr,  # noqa: ARG002
        event_edge: ScAddr,  # noqa: ARG002
        action_element: ScAddr,
    ) -> ScResult:
        try:
            result = self.run(action_element)
            is_successful = result == ScResult.OK
            finish_action_with_status(action_element, is_successful)
            self.logger.info(
                "Finished %s",
                "successfully" if is_successful else "unsuccessfully",
            )
            return result
        except Exception as e:
            self.logger.info("Finished with an error %s", e)
            return ScResult.ERROR

    def run(self, action_node: ScAddr) -> ScResult:
        message_addr = get_action_arguments(action_node, 1)[0]
        app_name_link = search_element_by_role_relation(
            message_addr,
            self.rrel_program,
        )
        if app_name_link is None:
            self.logger.error("Did not find app name!!!")
            return ScResult.ERROR
        app_name = get_link_content_data(app_name_link)
        result = self.launch_app(app_name)
        return ScResult.OK if result else ScResult.ERROR

    def launch_app(self, app_name: str) -> bool:
        try:
            subprocess.run([app_name], check=True)
            self.logger.info("Launch %s successfully!", app_name)
            return True
        except FileNotFoundError:
            self.logger.error("Did not find app %s", app_name)
            return False
        except subprocess.CalledProcessError as e:
            self.logger.error("error while launching app: %s", e)
            return False
