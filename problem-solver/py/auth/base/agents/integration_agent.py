from abc import ABC, abstractmethod

from sc_client.models import ScAddr
from sc_kpm import ScAgentClassic, ScKeynodes
from sc_kpm.identifiers import CommonIdentifiers
from sc_kpm.sc_sets import ScStructure
from sc_kpm.utils import (
    get_link_content_data,
    search_element_by_non_role_relation,
)
from sc_kpm.utils.action_utils import (
    execute_agent,
    get_action_result,
)

from auth.base.models import User
from auth.base.services import FernetCryptoService
from secrets_env import CRYPTO_KEY


class IntegrationAgent(ScAgentClassic, ABC):
    def __init__(self, action: str):
        super().__init__(action)
        self.nrel_name: ScAddr = ScKeynodes.get("nrel_name")
        self.nrel_email: ScAddr = ScKeynodes.get("nrel_email")
        self.author_node: ScAddr | None = None
        self._crypto_service = FernetCryptoService(CRYPTO_KEY)

    @property
    @abstractmethod
    def check_token_agent_action(self) -> str:
        pass

    def get_token(self) -> str | None:
        action, is_successful = execute_agent(
            arguments={self.author_node: False},
            concepts=[
                CommonIdentifiers.ACTION,
                self.check_token_agent_action,
            ],
        )
        if is_successful:
            result_struct = get_action_result(action)
            token_link = ScStructure(
                set_node=result_struct,
            ).elements_set.pop()
            access_token = get_link_content_data(token_link)
            return self._crypto_service.decrypt_token(access_token)
        return None

    def get_author(self) -> User:
        name = search_element_by_non_role_relation(
            self.author_node,
            self.nrel_name,
        )
        email = search_element_by_non_role_relation(
            self.author_node,
            self.nrel_email,
        )
        try:
            return User(
                name=get_link_content_data(name),
                email=get_link_content_data(email),
            )
        except ValueError:
            self.logger.info(
                f"Got error with author params: {name=}, {email=}",
            )
            return None
