import logging

from abc import ABC, abstractmethod

from sc_client.client import (
    search_by_template,
    set_link_contents,
)
from sc_client.constants import sc_type
from sc_client.models import (
    ScAddr,
    ScLinkContent,
    ScLinkContentType,
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


class CheckTokenAgent(ScAgentClassic, ABC):
    def __init__(self, action_class_name):
        super().__init__(action_class_name)
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
        event_connector: ScAddr,  # noqa: ARG002
        action_element: ScAddr,
    ) -> ScResult:
        if not self.service:
            return ScResult.ERROR
        args = get_action_arguments(action_element, 1)
        if not any(args):
            self.logger.error("Did not find necessary args")
            finish_action_with_status(action_element, is_success=False)
            return

        author_node = args[0]
        tokens_dict = self._get_tokens(author_node)
        if tokens_dict is None:
            self.logger.error("Did not find tokens!!!")

        access_token_link = tokens_dict.get("access_token")
        refresh_token_link = tokens_dict.get("refresh_token")

        access_token = get_link_content_data(access_token_link)
        refresh_token = get_link_content_data(refresh_token_link)

        if not self.service.is_token_valid(access_token):
            new_token = self.service.get_new_token(refresh_token)
            new_link_content = ScLinkContent(
                new_token,
                ScLinkContentType.STRING,
                generate_link(access_token),
            )
            set_link_contents([new_link_content])

        generate_action_result(action_element, (access_token_link))
        finish_action_with_status(action_element, is_success=True)

        return ScResult.OK

    def _get_tokens(self, author_node: ScAddr) -> dict[str, ScAddr]:
        template = ScTemplate()

        acs_alias = "_acs"
        ref_alias = "_ref"

        template.quintuple(
            author_node,
            sc_type.VAR_COMMON_ARC,
            sc_type.VAR_NODE_LINK >> acs_alias,
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_access_token,
        )
        template.quintuple(
            author_node,
            sc_type.VAR_COMMON_ARC,
            sc_type.VAR_NODE_LINK >> ref_alias,
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_refresh_token,
        )

        res = search_by_template(template)
        if res:
            return {
                "access_token": res[0].get(acs_alias),
                "refresh_token": res[0].get(ref_alias),
            }
