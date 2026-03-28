import logging

from abc import ABC, abstractmethod

from sc_client.client import generate_by_template
from sc_client.constants import sc_type
from sc_client.models import (
    ScAddr,
    ScTemplate,
)
from sc_kpm import ScAgentClassic, ScKeynodes, ScResult
from sc_kpm.utils import (
    generate_link,
    get_link_content_data,
)
from sc_kpm.utils.action_utils import (
    finish_action_with_status,
    generate_action_result,
    get_action_arguments,
)

from auth.base.services import OauthTokenService


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class CreateTokensAgent(ScAgentClassic, ABC):
    def __init__(self, action: str):
        super().__init__(action)
        self.nrel_refresh_token = ScKeynodes.resolve(
            "nrel_refresh_token",
            sc_type.CONST_NODE_NON_ROLE,
        )
        self.nrel_access_token = ScKeynodes.resolve(
            "nrel_access_token",
            sc_type.CONST_NODE_NON_ROLE,
        )

    @property
    @abstractmethod
    def service(self) -> OauthTokenService:
        pass

    def on_event(
        self,
        event_element: ScAddr,  # noqa: ARG002
        event_edge: ScAddr,  # noqa: ARG002
        action_element: ScAddr,
    ) -> ScResult:
        try:
            args = get_action_arguments(action_element, 2)
            if not any(args):
                self.logger.error("Did not find necessary args")
                finish_action_with_status(action_element, is_success=False)
                return

            author_node = args[0]
            code = get_link_content_data(args[1])
            self.logger.info("Found all necessary args.")

            token_dict = self.service.get_tokens(code)

            if not token_dict:
                return ScResult.ERROR
            access_token_link = generate_link(
                token_dict.get("access_token", ""),
            )
            refresh_token_link = generate_link(
                token_dict.get("refresh_token", ""),
            )
            if not any([access_token_link, refresh_token_link]):
                self.logger.error("Did not create tokens links!!!")
                finish_action_with_status(action_element, is_success=False)
                return
            self.logger.info("Created tokens links.")

            if self._save_tokens(
                author_node,
                access_token_link,
                refresh_token_link,
            ):
                generate_action_result(
                    action_element,
                    access_token_link,
                )
                finish_action_with_status(action_element, is_success=True)

            finish_action_with_status(action_element, is_success=False)

        except Exception as e:
            self.logger.error("Finished with error: %s", e)
            finish_action_with_status(action_element, is_success=False)

    def _save_tokens(
        self,
        author_node: ScAddr,
        access_token_link: ScAddr,
        refresh_token_link: ScAddr,
    ):
        template = ScTemplate()

        template.quintuple(
            author_node,
            sc_type.VAR_COMMON_ARC,
            access_token_link,
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_access_token,
        )
        template.quintuple(
            author_node,
            sc_type.VAR_COMMON_ARC,
            refresh_token_link,
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_refresh_token,
        )

        return generate_by_template(template)
