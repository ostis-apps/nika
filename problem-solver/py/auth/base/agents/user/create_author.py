import logging

from abc import ABC, abstractmethod

from sc_client.client import generate_by_template
from sc_client.constants import sc_type
from sc_client.models import ScAddr, ScTemplate
from sc_kpm import ScAgentClassic, ScKeynodes, ScResult
from sc_kpm.identifiers import CommonIdentifiers
from sc_kpm.sc_sets import ScStructure
from sc_kpm.utils import (
    generate_link,
    get_link_content_data,
)
from sc_kpm.utils.action_utils import (
    execute_agent,
    finish_action_with_status,
    get_action_arguments,
    get_action_result,
)

from auth.base.models import User
from auth.base.services import OauthUserService


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class CreateAuthorAgent(ScAgentClassic, ABC):
    def __init__(self, action: str):
        super().__init__(action)
        self.lang_en: ScAddr = ScKeynodes.get("lang_en")
        self.concept_dialogue: ScAddr = ScKeynodes.resolve(
            "concept_dialogue",
            sc_type.NODE_CLASS,
        )
        self.rrel_dialog_participant: ScAddr = ScKeynodes.resolve(
            "rrel_dialog_participant",
            sc_type.NODE_ROLE,
        )
        self.nrel_name: ScAddr = ScKeynodes.resolve(
            "nrel_name",
            sc_type.CONST_NODE_NON_ROLE,
        )
        self.nrel_email: ScAddr = ScKeynodes.resolve(
            "nrel_email",
            sc_type.CONST_NODE_NON_ROLE,
        )
        self.nrel_auth_session: ScAddr = ScKeynodes.resolve(
            "nrel_auth_session",
            sc_type.CONST_NODE_NON_ROLE,
        )
        self.concept_user: ScAddr = ScKeynodes.resolve(
            "concept_user",
            sc_type.CONST_NODE_CLASS,
        )

    @property
    @abstractmethod
    def service(self) -> OauthUserService:
        pass

    @property
    @abstractmethod
    def token_agent_action(self) -> str:
        pass

    def on_event(
        self,
        event_element: ScAddr,  # noqa: ARG002
        event_edge: ScAddr,  # noqa: ARG002
        action_element: ScAddr,
    ) -> ScResult:
        # rrel1: session from browser
        # rrel2: query param code from oauth service
        args = get_action_arguments(action_element, 2)
        auth_session_link = args[0]
        code_link = args[1]

        self.author_node = self._create_author_node()
        if self.author_node is None:
            finish_action_with_status(action_element, is_success=False)
            return

        token = self._get_token(code_link)
        if token is None:
            finish_action_with_status(action_element, is_success=False)
            return

        author = self.service.get_user(token)
        res = self._save_author(author, auth_session_link)
        if not res:
            finish_action_with_status(action_element, is_success=False)

        finish_action_with_status(action_element, is_success=True)

    def _get_token(self, code_link: ScAddr) -> str:
        token_action, is_successful = execute_agent(
            arguments={
                self.author_node: False,
                code_link: False,
            },
            concepts=[CommonIdentifiers.ACTION, self.token_agent_action],
        )
        if not is_successful:
            return None

        res = get_action_result(token_action)
        token_link = ScStructure(
            set_node=res,
        ).elements_set.pop()
        token = get_link_content_data(token_link)
        return token

    def _save_author(
        self,
        author: User,
        session_link: str,
    ):
        name_link = generate_link(author.name)
        email_link = generate_link(author.email)

        template = ScTemplate()

        template.triple(
            self.lang_en,
            sc_type.VAR_PERM_POS_ARC,
            session_link,
        )
        # generate all links connected with this user
        template.quintuple(
            self.author_node,
            sc_type.VAR_COMMON_ARC,
            name_link,
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_name,
        )
        template.quintuple(
            self.author_node,
            sc_type.VAR_COMMON_ARC,
            email_link,
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_email,
        )
        template.quintuple(
            self.author_node,
            sc_type.VAR_COMMON_ARC,
            session_link,
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_auth_session,
        )
        return generate_by_template(template)

    def _create_author_node(self):
        template = ScTemplate()
        user_alias = "_user"
        dialog = "_dialogue"

        # generate new user node
        template.triple(
            self.concept_user,
            sc_type.VAR_PERM_POS_ARC,
            sc_type.VAR_NODE >> user_alias,
        )

        template.triple(
            self.concept_dialogue,
            sc_type.VAR_PERM_POS_ARC,
            sc_type.VAR_NODE >> dialog,
        )

        template.quintuple(
            dialog,
            sc_type.VAR_PERM_POS_ARC,
            user_alias,
            sc_type.VAR_PERM_POS_ARC,
            self.rrel_dialog_participant,
        )
        res = generate_by_template(template)
        if res:
            return res.get(user_alias)
